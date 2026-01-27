# Eligibility Matrix System - PhD Portal Analysis & Department-Only Proposal

## Executive Summary

This document provides a comprehensive analysis of the **PhD Portal** eligibility system, compares it with the original 130-bit program×department matrix system, and outlines the changes required to implement **department-only eligibility** (where selecting a department makes all students in that department eligible, regardless of their specific research keyword).

---

## Table of Contents

1. [Current PhD Portal System Overview](#current-phd-portal-system-overview)
2. [Key Differences from Original 130-Bit System](#key-differences-from-original-130-bit-system)
3. [End-to-End Flow in PhD Portal](#end-to-end-flow-in-phd-portal)
4. [Department-Only Eligibility Proposal](#department-only-eligibility-proposal)
5. [Required Code Changes](#required-code-changes)
6. [Who Sees What After Changes](#who-sees-what-after-changes)
7. [Code References](#code-references)

---

## Current PhD Portal System Overview

### Eligibility Matrix Structure

**String Format:**
- **Length**: 223 characters (positions 0-222)
- **Index 0**: Always set to `'0'` (reserved/unused) via `BeforeUpdate` hook
- **Positions 1-222**: Map to specific **Department-Keyword** combinations
- **Default Value**: `"0000...0"` (130 zeros) - **BUG**: Should be 223 zeros!

### Mapping Structure

The PhD portal uses a **Department → Research Keyword** mapping:

- **20 Departments** (e.g., `CSE`, `EE`, `ME`, `MSE`, `CHE`, `BSBE`, etc.)
- **222 Research Keywords** total across all departments
- Each keyword maps to a unique position (1-222) in the eligibility string

**Example Mapping:**
```
Position 99:  "CSE-Theoretical-computer-science"
Position 100: "CSE-Machine-learning"
Position 101: "CSE-Computer-systems"
...
Position 5:   "MSE-Opto-electronic-devices"
Position 17:  "ME-Combustion"
```

**Key Insight**: In the PhD portal, `ProgramDepartmentID` represents a **keyword ID** (1-222), not a program×department combo. The "department" is extracted from the keyword ID using the reverse mapping.

---

## Key Differences from Original 130-Bit System

| Aspect | Original 130-Bit System | PhD Portal System |
|--------|-------------------------|-------------------|
| **String Length** | 130 characters | 223 characters |
| **Position Meaning** | Program×Department combo (e.g., BT-CSE) | Department×Keyword combo (e.g., CSE-Machine-learning) |
| **Query Method** | SQL `SUBSTRING(eligibility FROM id+1 FOR 1) = '1'` | SQL `LIKE` with wildcard patterns `'____1____%'` |
| **Default Initialization** | 130 zeros | **130 zeros (BUG - should be 223)** |
| **Student ID Meaning** | Program×Department ID | Keyword ID (research specialization) |
| **Company Selection** | Program×Department matrix | Department×Keyword matrix |

### Critical Bug Identified

**File**: `Phd-PAS-backend/application/model.hooks.go` (line 58)

```go
func (p *Proforma) BeforeCreate(tx *gorm.DB) (err error) {
    p.Eligibility = strings.Repeat("0", 130)  // ❌ WRONG: Should be 223!
    return
}
```

**Impact**: New proformas are initialized with only 130 characters, but the system expects 223. This causes:
- Eligibility checks to fail for students with keyword IDs > 130
- Potential index out of bounds errors
- Inconsistent behavior

**Fix Required**: Change to `strings.Repeat("0", 223)` or use a constant.

---

## End-to-End Flow in PhD Portal

### A. Company Creates/Updates Eligibility

#### Step 1: Frontend UI Selection
**File**: `PhD-PAS-frontend/pages/company/rc/[rcid]/proforma/[proformaid]/step2.tsx`

**Current Behavior:**
- Displays a table with **20 departments** as rows
- Under each department, shows **all research keywords** for that department
- Each keyword has a checkbox
- Company can:
  - Select individual keywords
  - Select all keywords in a department (via department button)
  - Select all keywords (via "Select all" button)

**State Management:**
```typescript
const [str, setStr] = useState(new Array(totalDeptKeywords + 1).join("0")); // 223 zeros
```

**Key Functions:**
- `handleCheck(branch, keyword)`: Toggles individual keyword eligibility
- `handleBranchWise(branchName)`: Selects all keywords for a department
- `handleCheckAll()`: Sets all positions to `'1'`
- `handleReset()`: Sets all positions to `'0'`

**On Submit:**
```typescript
const info = {
  eligibility: str,  // 223-character string
  ID: parseInt(pid, 10),
} as ProformaType;
await proformaRequest.put(token, rid, info);
```

#### Step 2: API Request
**File**: `PhD-PAS-frontend/callbacks/company/proforma.ts`

- **Endpoint**: `PUT /api/company/application/rc/:rid/proforma`
- **Payload**: `{ ID: number, eligibility: string }`

#### Step 3: Backend Processing
**File**: `Phd-PAS-backend/application/company.proforma.go`

```go
func putProformaByCompanyHandler(ctx *gin.Context) {
    // ... validation ...
    var jp Proforma
    ctx.ShouldBindJSON(&jp)
    jp.CompanyRecruitmentCycleID = cid
    ok, err := updateProformaForCompany(ctx, &jp)
    // ...
}
```

#### Step 4: Database Update
**File**: `Phd-PAS-backend/application/db.proforma.go`

```go
func updateProformaForCompany(ctx *gin.Context, jp *Proforma) (bool, error) {
    tx := db.WithContext(ctx).
        Where("id = ? AND company_recruitment_cycle_id = ?", jp.ID, jp.CompanyRecruitmentCycleID).
        Updates(jp)
    return tx.RowsAffected == 1, tx.Error
}
```

**BeforeUpdate Hook** (ensures index 0 is always '0'):
```go
func (p *Proforma) BeforeUpdate(tx *gorm.DB) (err error) {
    if p.Eligibility != "" {
        p.Eligibility = "0" + p.Eligibility[1:]
    }
    return
}
```

#### Step 5: Database Storage
**Table**: `proforma`
- **Field**: `eligibility VARCHAR(223)` (or larger)
- **Stored Value**: Binary string like `"0101010101..."` (223 characters)

---

### B. Student Views Eligible Openings

#### Step 1: Student Request
**Endpoint**: `GET /api/student/application/rc/:rid/opening`

**File**: `Phd-PAS-backend/application/student.proforma.go`

```go
func getProformasForEligibleStudentHandler(ctx *gin.Context) {
    rid := parseUint(ctx.Param("rid"))
    sid := getStudentRCID(ctx)
    var student rc.StudentRecruitmentCycle
    rc.FetchStudent(ctx, sid, &student)
    var jps []Proforma
    fetchProformaForEligibleStudent(ctx, rid, &student, &jps)
    ctx.JSON(http.StatusOK, jps)
}
```

#### Step 2: Eligibility Filtering
**File**: `Phd-PAS-backend/application/db.proforma.go` (lines 162-192)

**Key Logic:**
```go
func fetchProformaForEligibleStudent(ctx *gin.Context, rid uint, student *rc.StudentRecruitmentCycle, jps *[]Proforma) error {
    // Build wildcard pattern for primary keyword ID
    eligibility := bytes.Repeat([]byte("_"), 223)
    eligibility[student.ProgramDepartmentID] = byte('1')
    
    // Build wildcard pattern for secondary keyword ID
    secondary_eligibility := bytes.Repeat([]byte("_"), 223)
    secondary_eligibility[student.SecondaryProgramDepartmentID] = byte('1')
    
    // Get student's PhD stage
    stageFetch, _ := extractStudentStageofPhD(ctx)
    
    // Query proformas
    tx := db.WithContext(ctx).
        Where(
            "recruitment_cycle_id = ? AND is_approved = ? AND deadline > ? AND " +
            "(eligibility LIKE ? OR eligibility LIKE ?) AND " +
            "cpi_cutoff <= ? AND id NOT IN (?) AND " +
            "additional_eligibility LIKE ?",
            rid, true, time.Now().UnixMilli(),
            string(eligibility)+"%",      // Pattern: "____1____%"
            string(secondary_eligibility)+"%",
            student.CPI, subQuery, "%"+stageFetch+"%"
        ).
        Find(jps)
    return tx.Error
}
```

**How It Works:**
1. Creates a 223-char string filled with `'_'` (wildcard)
2. Sets position `ProgramDepartmentID` to `'1'`
3. Uses SQL `LIKE` to match proformas where that position is `'1'`
4. Also checks secondary keyword ID (for dual/double major students)
5. Filters by CPI cutoff, deadline, not already applied, and PhD stage

**Example:**
- Student has `ProgramDepartmentID = 100` (CSE-Machine-learning)
- Pattern created: `"________________...1________________...%"`
- SQL: `eligibility LIKE '________________...1________________...%'`
- Matches proformas where position 100 is `'1'`

#### Step 3: Additional Eligibility Check
**File**: `Phd-PAS-backend/application/util.student_rcid.go`

```go
func extractStudentStageofPhD(ctx *gin.Context) (string, error) {
    user_email := middleware.GetUserID(ctx)
    var stud student.Student
    student.GetStudentByEmail(ctx, &stud, user_email)
    return stud.Specialization, nil  // Returns PhD stage
}
```

**PhD Stages** (from `matrixUtils.ts`):
- "Open seminar given.Thesis yet to submit"
- "Thesis submitted. Yet to defend"
- "Thesis defended. Degree will be awarded in next convocation"
- "PhD degree awarded in current year"

Companies select eligible stages in **Step 4** (`additional_eligibility` field).

---

### C. Student Opens Individual Proforma

**Endpoint**: `GET /api/student/application/rc/:rid/opening/:pid`

**File**: `Phd-PAS-backend/application/student.proforma.go`

```go
func getProformaForStudentHandler(ctx *gin.Context) {
    pid := parseUint(ctx.Param("pid"))
    var jp Proforma
    fetchProformaForStudent(ctx, pid, &jp)
    ctx.JSON(http.StatusOK, jp)
}
```

**Eligibility Display:**
- Frontend displays eligibility matrix using `MatrixExpanded` component
- Shows checkmarks (✓) for eligible keywords, crosses (✗) for non-eligible

---

## Department-Only Eligibility Proposal

### Goal
**Change the system so that companies select only departments** (not individual keywords). When a department is selected, **all students in that department become eligible**, regardless of their specific research keyword.

### Design Approach

**Keep the existing 223-character eligibility string**, but change the **company UI** to:
- Show only **department checkboxes** (no keyword-level selection)
- When a department is checked, **automatically set all keyword positions for that department to `'1'`**
- When a department is unchecked, **set all keyword positions for that department to `'0'`**

**Why This Works:**
- Backend filtering logic remains unchanged (still checks keyword ID positions)
- Student eligibility still determined by their `ProgramDepartmentID` (keyword ID)
- If department is selected → all keywords in dept are `'1'` → any student in that dept matches
- No database schema changes required
- Minimal code changes

---

## Required Code Changes

### 1. Frontend: Simplify Step2 UI to Department-Only

**File**: `PhD-PAS-frontend/pages/company/rc/[rcid]/proforma/[proformaid]/step2.tsx`

#### Current Implementation (Keyword-Level Selection)
```typescript
// Shows departments with nested keyword checkboxes
{Branches.map((branch) => (
  <TableRow>
    <TableCell>{branch}</TableCell>
    <TableCell>
      {Object.keys(func[branch]).map((keyword) => (
        <Checkbox
          checked={str[parseInt(func[branch][keyword], 10)] === "1"}
          onClick={() => handleCheck(branch, keyword)}
        />
      ))}
    </TableCell>
  </TableRow>
))}
```

#### Proposed Implementation (Department-Only Selection)
```typescript
// Show only department checkboxes
{Branches.map((branch) => {
  const keywordIndices = Object.values(func[branch as keyof typeof func])
    .map(idx => parseInt(idx, 10));
  const isDeptSelected = keywordIndices.every(idx => str[idx] === "1");
  
  return (
    <TableRow key={branch}>
      <TableCell>
        <Checkbox
          checked={isDeptSelected}
          onChange={() => handleDepartmentToggle(branch)}
        />
      </TableCell>
      <TableCell sx={{ fontWeight: 600 }}>{branch}</TableCell>
    </TableRow>
  );
})}
```

#### New Handler Function
```typescript
const handleDepartmentToggle = (branchName: string) => {
  const keywordIndices = Object.values(func[branchName as keyof typeof func])
    .map(idx => parseInt(idx, 10));
  
  // Check if department is currently fully selected
  const isCurrentlySelected = keywordIndices.every(idx => str[idx] === "1");
  
  let newStr = str;
  keywordIndices.forEach(idx => {
    newStr = `${newStr.substring(0, idx)}${isCurrentlySelected ? "0" : "1"}${newStr.substring(idx + 1)}`;
  });
  
  setStr(newStr);
};
```

#### Remove/Modify Functions
- **Keep**: `handleBranchWise()` (can be repurposed or removed)
- **Remove**: `handleCheck()` (individual keyword selection)
- **Keep**: `handleCheckAll()` and `handleReset()` (still useful)

---

### 2. Backend: Fix Default Eligibility Length Bug

**File**: `Phd-PAS-backend/application/model.hooks.go`

#### Current Code (BUG)
```go
func (p *Proforma) BeforeCreate(tx *gorm.DB) (err error) {
    p.Eligibility = strings.Repeat("0", 130)  // ❌ WRONG
    return
}
```

#### Fixed Code
```go
const EligibilityStringLength = 223  // Add constant at package level

func (p *Proforma) BeforeCreate(tx *gorm.DB) (err error) {
    p.Eligibility = strings.Repeat("0", EligibilityStringLength)  // ✅ FIXED
    return
}
```

**Alternative**: Use `totalDeptKeywords + 1` if importing from a shared constant file.

---

### 3. Optional: Update Eligibility Display Component

**File**: `PhD-PAS-frontend/components/Utils/MatrixExpanded.tsx`

**Current**: Shows department rows with nested keyword rows (each with ✓/✗)

**Proposed**: Show only department rows with ✓/✗ based on whether **all** keywords in that department are selected.

```typescript
function MatrixExpanded({ data }: { data: string }) {
  return (
    <TableContainer component={Paper}>
      <Table>
        <TableBody>
          {Branches.map((branch) => {
            const keywordIndices = Object.values(func[branch as keyof typeof func])
              .map(idx => parseInt(idx, 10));
            const isDeptSelected = keywordIndices.every(idx => data[idx] === "1");
            
            return (
              <TableRow key={branch}>
                <TableCell component="th" scope="row" sx={{ fontWeight: 600 }}>
                  {branch}
                </TableCell>
                <TableCell>
                  {isDeptSelected ? (
                    <CheckIcon sx={{ color: "green" }} />
                  ) : (
                    <CloseIcon sx={{ color: "red" }} />
                  )}
                </TableCell>
              </TableRow>
            );
          })}
        </TableBody>
      </Table>
    </TableContainer>
  );
}
```

---

## Who Sees What After Changes

### Companies
- **Step 2 UI**: See only **department checkboxes** (20 departments)
- **Selection**: Check/uncheck entire departments
- **Storage**: When department is checked, all keyword positions for that department are set to `'1'` in the eligibility string
- **View Proforma**: See simplified eligibility matrix (department-level only, if display component is updated)

### Students
- **Eligibility Check**: Still based on their `ProgramDepartmentID` (keyword ID)
- **If their department is selected**: Their keyword position will be `'1'` → **ELIGIBLE** ✓
- **View Openings**: See proformas where their department was selected
- **Apply**: Can apply if other constraints are met (CPI, deadline, PhD stage, not already applied)

### Admins
- **View Proforma**: See eligibility matrix (department-level if display updated)
- **Edit Proforma**: Can still edit eligibility string directly (if needed)

---

## Code References

### Frontend Files

1. **Eligibility Selection UI**
   - `PhD-PAS-frontend/pages/company/rc/[rcid]/proforma/[proformaid]/step2.tsx`
   - Main UI for company to select eligibility (needs modification)

2. **Eligibility Display Component**
   - `PhD-PAS-frontend/components/Utils/MatrixExpanded.tsx`
   - Displays eligibility matrix (optional update)

3. **Mapping Configuration**
   - `PhD-PAS-frontend/components/Utils/matrixUtils.ts`
   - Contains `func` object (department → keyword → index)
   - Contains `rev` object (index → "DEPT-KEYWORD")
   - Contains `Branches` array (20 departments)
   - Contains `totalDeptKeywords = 222`

4. **Parser Utilities**
   - `PhD-PAS-frontend/components/Parser/parser.ts`
   - `getDepartment(id)`: Extracts department from keyword ID
   - `getProgram(id)`: Extracts keyword from keyword ID
   - `getDeptProgram(id)`: Returns full "DEPT-KEYWORD" string

5. **API Callbacks**
   - `PhD-PAS-frontend/callbacks/company/proforma.ts`
   - PUT request to update eligibility

### Backend Files

1. **Model Definition**
   - `Phd-PAS-backend/application/model.go`
   - `Proforma` struct with `Eligibility string` field

2. **Database Hooks**
   - `Phd-PAS-backend/application/model.hooks.go`
   - `BeforeCreate`: Sets default eligibility (**BUG: 130 should be 223**)
   - `BeforeUpdate`: Ensures index 0 is always `'0'`

3. **Company Handlers**
   - `Phd-PAS-backend/application/company.proforma.go`
   - `putProformaByCompanyHandler()`: Handles PUT request

4. **Database Functions**
   - `Phd-PAS-backend/application/db.proforma.go`
   - `updateProformaForCompany()`: Updates eligibility in DB
   - `fetchProformaForEligibleStudent()`: Queries eligible proformas using `LIKE` patterns

5. **Student Eligibility Check**
   - `Phd-PAS-backend/rc/db.student.go`
   - `GetStudentEligible()`: Verifies student eligibility (checks keyword ID position)

6. **Student Handlers**
   - `Phd-PAS-backend/application/student.proforma.go`
   - `getProformasForEligibleStudentHandler()`: Returns eligible openings

7. **Utility Functions**
   - `Phd-PAS-backend/application/util.student_rcid.go`
   - `extractStudentStageofPhD()`: Gets student's PhD stage

8. **Student Model**
   - `Phd-PAS-backend/student/model.go`
   - `Student` struct with `ProgramDepartmentID` and `Specialization` fields

9. **RC Model**
   - `Phd-PAS-backend/rc/model.go`
   - `StudentRecruitmentCycle` struct with `ProgramDepartmentID` and `SecondaryProgramDepartmentID`

---

## Summary

### Current System (PhD Portal)
- **Eligibility String**: 223 characters (positions 0-222)
- **Position Meaning**: Department-Keyword combinations
- **Company Selection**: Department × Keyword matrix (individual keyword checkboxes)
- **Student Matching**: Based on keyword ID (`ProgramDepartmentID`)
- **Query Method**: SQL `LIKE` with wildcard patterns
- **Bug**: Default initialization is 130 zeros (should be 223)

### Proposed System (Department-Only)
- **Eligibility String**: Same 223 characters (no schema change)
- **Position Meaning**: Same (Department-Keyword combinations)
- **Company Selection**: **Department-only checkboxes** (no keyword-level selection)
- **Student Matching**: Same (based on keyword ID)
- **Query Method**: Same (SQL `LIKE` patterns)
- **Fix**: Default initialization corrected to 223 zeros

### Key Benefits
1. **Simpler UI**: Companies only select departments, not individual keywords
2. **Automatic Coverage**: Selecting a department makes all students in that department eligible
3. **Backward Compatible**: Backend logic unchanged, only UI changes
4. **Bug Fix**: Corrects the 130 vs 223 initialization issue

### Implementation Priority
1. **High**: Fix backend default eligibility length (prevents bugs)
2. **High**: Update Step2 UI to department-only selection
3. **Medium**: Update MatrixExpanded display component (optional, improves UX)
4. **Low**: Add migration script to fix existing proformas with 130-char eligibility (if any)

---

## Example: CSE Department Selection

### Scenario
Company wants to allow **all CSE students** to apply, regardless of their research keyword.

### Current Flow (Keyword-Level)
1. Company opens Step 2
2. Sees CSE department with 12 keywords listed:
   - Theoretical-computer-science (position 99)
   - Machine-learning (position 100)
   - Computer-systems (position 101)
   - ... (9 more keywords)
3. Company checks all 12 keyword checkboxes individually
4. Eligibility string: Positions 99-110 set to `'1'`

### Proposed Flow (Department-Level)
1. Company opens Step 2
2. Sees CSE department with **single checkbox**
3. Company checks the CSE checkbox
4. **Automatically**: All 12 keyword positions (99-110) set to `'1'`
5. Eligibility string: Same result, but achieved with one click

### Student Matching (Unchanged)
- Student with `ProgramDepartmentID = 100` (CSE-Machine-learning)
- Backend checks: `eligibility[100] == '1'` → **ELIGIBLE** ✓
- Works the same way in both systems

---

## Conclusion

The PhD portal eligibility system uses a **223-character string** where positions represent **Department-Keyword combinations**, queried using SQL `LIKE` patterns. The proposed department-only selection simplifies the company UI while maintaining backend compatibility. The critical bug fix (130 → 223 default length) should be implemented immediately to prevent eligibility check failures.

