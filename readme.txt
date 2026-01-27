# Eligibility Matrix System - End-to-End Report

## Executive Summary

This document provides a comprehensive end-to-end analysis of how companies select departments and programs (e.g., BT CSE) for job openings, how this information is stored in the database, and how student eligibility is verified. The system uses an **Eligibility Matrix** - a binary string representation that maps program-department combinations to specific positions in a string.

---

## Table of Contents

1. [Overview](#overview)
2. [Eligibility Matrix Structure](#eligibility-matrix-structure)
3. [Program-Department Mapping](#program-department-mapping)
4. [Frontend: Company Selection Process](#frontend-company-selection-process)
5. [Backend: API Endpoints](#backend-api-endpoints)
6. [Database Storage](#database-storage)
7. [Student Eligibility Verification](#student-eligibility-verification)
8. [Complete Flow Diagram](#complete-flow-diagram)
9. [Code References](#code-references)

---

## Overview

The Eligibility Matrix is a **130-character binary string** where each position represents a specific program-department combination. A `'1'` indicates eligibility, and `'0'` indicates non-eligibility.

**Key Components:**
- **Frontend**: Interactive matrix UI for companies to select eligible programs/departments
- **Backend**: API endpoints to save/update eligibility matrix
- **Database**: Stores eligibility as a string in the `proforma` table
- **Verification**: Students are checked against this matrix when viewing/applying to openings

---

## Eligibility Matrix Structure

### String Format
- **Length**: 130 characters (positions 0-129)
- **Index 0**: Always set to `'0'` (reserved/unused)
- **Positions 1-139**: Map to specific program-department combinations
- **Default Value**: `"0000...0"` (130 zeros) - no eligibility by default

### Example
```
Position:  0  1  2  3  4  5  6  7  8  9  ...
Value:     0  1  1  0  0  1  0  0  0  0  ...
Meaning:   -  BT BT BT BT BT BT BT BT BT ...
           -  AE BSBE CE CHE CSE EE MSE ME ...
```

---

## Program-Department Mapping

### Programs Supported
1. **BT** - Bachelor of Technology
2. **BS** - Bachelor of Science
3. **BTM** - Bachelor of Technology (Minor)
4. **BSM** - Bachelor of Science (Minor)
5. **DoubleMajor** - Double Major programs
6. **DualA** - Dual Degree Type A
7. **DualB** - Dual Degree Type B
8. **DualC** - Dual Degree Type C
9. **MT** - Master of Technology
10. **MSR** - Master of Science by Research
11. **MSc** - Master of Science
12. **MDes** - Master of Design
13. **MBA** - Master of Business Administration
14. **PhD** - Doctor of Philosophy

### Departments Supported
27 departments including:
- **AE** - Aerospace Engineering
- **BSBE** - Biological Sciences and Bioengineering
- **CE** - Civil Engineering
- **CHE** - Chemical Engineering
- **CSE** - Computer Science and Engineering
- **EE** - Electrical Engineering
- **MSE** - Material Science and Engineering
- **ME** - Mechanical Engineering
- **CHM** - Chemistry
- **ECO** - Economics
- **MTH** - Mathematics and Scientific Computing
- **SDS** - Statistics and Data Science
- **PHY** - Physics
- And 14 more...

### Mapping Table (Key Examples)

| Position | Program-Department | Example |
|----------|-------------------|---------|
| 1 | BT-AE | Bachelor of Technology - Aerospace Engineering |
| 5 | BT-CSE | Bachelor of Technology - Computer Science and Engineering |
| 18 | MT-CSE | Master of Technology - Computer Science and Engineering |
| 32 | DoubleMajor-CSE | Double Major - Computer Science and Engineering |
| 42 | DualA-CSE | Dual Degree Type A - Computer Science and Engineering |

**Full mapping**: See `ras-frontend/components/Utils/matrixUtils.ts` - `func` object (lines 105-538)

---

## Frontend: Company Selection Process

### Step 3: Eligibility Matrix Selection

**File**: `ras-frontend/pages/company/rc/[rcid]/proforma/[proformaid]/step3.tsx`

#### UI Components

1. **Matrix Table**
   - Rows: Departments (27 branches)
   - Columns: Programs (14 program types)
   - Checkboxes: Allow selection of specific combinations

2. **Quick Selection Buttons**
   - **Select All**: Sets all positions to `'1'`
   - **JEE Advanced**: Selects BT, BS, DoubleMajor, DualA, DualB, DualC, BTM, BSM
   - **GATE**: Selects MT, MSR, MDes
   - **JAM**: Selects MSc
   - **CAT**: Selects MBA
   - **CEED**: Selects MDes

3. **Program-Wise Selection**
   - Buttons to select all departments for a program type
   - Example: "Bachelor of Technology / Bachelor of Science / Double Major" button

4. **Branch-Wise Selection**
   - Buttons to select all programs for a department
   - Example: "Computer Science and Engineering (CSE)" button

#### State Management

```typescript
const [str, setStr] = useState(new Array(133 + 1).join("0")); // Initialize with zeros
```

#### Key Functions

1. **`handleCheck(branch, program)`**
   - Toggles eligibility for specific branch-program combinations
   - Uses `func` mapping to find correct string index
   - Updates string: `newStr = newStr.substring(0, idx) + "1" + newStr.substring(idx + 1)`

2. **`handleProgramWise(programName[])`**
   - Selects all branches for given program types
   - Iterates through all branches and sets corresponding indices

3. **`handleBranchWise(branchName)`**
   - Selects all programs for a given branch
   - Iterates through all program types

4. **`handleNext()`**
   - Sends eligibility string to backend via PUT request
   - API: `PUT /api/company/application/rc/:rid/proforma`
   - Payload: `{ eligibility: str, ID: proformaId }`

#### Matrix Display Component

**File**: `ras-frontend/components/Utils/MatrixExpanded.tsx`

- Displays eligibility matrix as a table
- Shows checkmarks (✓) for eligible combinations
- Shows crosses (✗) for non-eligible combinations
- Shows dashes (-) for invalid combinations

---

## Backend: API Endpoints

### Company Endpoints

#### 1. Create Proforma
**Endpoint**: `POST /api/company/application/rc/:rid/proforma`

**File**: `ras-backend/application/company.proforma.go` (lines 31-67)

**Process**:
1. Company creates new proforma
2. Default eligibility: `"0000...0"` (130 zeros) - set by `BeforeCreate` hook
3. Returns proforma ID

#### 2. Update Proforma (Eligibility)
**Endpoint**: `PUT /api/company/application/rc/:rid/proforma`

**File**: `ras-backend/application/company.proforma.go` (lines 69-97)

**Request Body**:
```json
{
  "ID": 123,
  "eligibility": "0101010101...",
  // ... other fields
}
```

**Process**:
1. Validates company authorization
2. Calls `updateProformaForCompany()`
3. Before update hook sets index 0 to `'0'` (always)
4. Updates database

**File**: `ras-backend/application/db.proforma.go` (lines 258-261)

#### 3. Get Proforma
**Endpoint**: `GET /api/company/application/rc/:rid/proforma/:pid`

**Returns**: Proforma object including eligibility string

---

### Student Endpoints

#### 1. Get Eligible Openings
**Endpoint**: `GET /api/student/application/rc/:rid/opening`

**File**: `ras-backend/application/student.proforma.go` (lines 29-54)

**Process**:
1. Gets student's recruitment cycle ID
2. Fetches student record (includes `ProgramDepartmentID` and `SecondaryProgramDepartmentID`)
3. Calls `fetchProformaForEligibleStudent()` which filters proformas based on eligibility matrix

#### 2. Check Eligibility Before Application
**Endpoint**: `POST /api/student/application/rc/:rid/opening/:pid`

**File**: `ras-backend/application/student.application.go` (lines 60-83)

**Process**:
1. Gets proforma eligibility string
2. Calls `rc.GetStudentEligible()` to verify student eligibility
3. Checks:
   - Student verification status
   - CPI cutoff
   - Program-department eligibility

---

## Database Storage

### Table: `proforma`

**File**: `ras-backend/application/model.go` (line 18)

```sql
CREATE TABLE proforma (
    id BIGINT PRIMARY KEY,
    eligibility VARCHAR(130) NOT NULL,
    cpi_cutoff DECIMAL(10,2),
    -- ... other fields
);
```

### Field Details

- **`eligibility`**: VARCHAR(130)
  - Stores binary string representation
  - Default: `"0000...0"` (130 zeros)
  - Index 0 always set to `'0'` before save (via `BeforeUpdate` hook)

### Database Hooks

**File**: `ras-backend/application/model.hooks.go`

#### BeforeCreate Hook (lines 57-60)
```go
func (p *Proforma) BeforeCreate(tx *gorm.DB) (err error) {
    p.Eligibility = strings.Repeat("0", 130)
    return
}
```
- Sets default eligibility to all zeros (no eligibility)

#### BeforeUpdate Hook (lines 49-54)
```go
func (p *Proforma) BeforeUpdate(tx *gorm.DB) (err error) {
    if p.Eligibility != "" {
        p.Eligibility = "0" + p.Eligibility[1:]
    }
    return
}
```
- Ensures index 0 is always `'0'` (reserved position)

---

## Student Eligibility Verification

### Student Program-Department IDs

**File**: `ras-backend/student/model.go` (lines 24-25)

Each student has:
- **`ProgramDepartmentID`**: Primary program-department (e.g., 5 for BT-CSE)
- **`SecondaryProgramDepartmentID`**: Secondary program-department (for dual/double major students)

### Verification Function

**File**: `ras-backend/rc/db.student.go` (lines 148-177)

```go
func GetStudentEligible(ctx *gin.Context, sid uint, eligibility string, cpiEligibility float64) (bool, error) {
    // Get student record
    var student StudentRecruitmentCycle
    tx := db.WithContext(ctx).Model(&StudentRecruitmentCycle{}).Where("id = ?", sid).First(&student)
    
    primaryID = int(student.ProgramDepartmentID)
    secondaryID = int(student.SecondaryProgramDepartmentID)
    
    // Check verification status
    if !student.IsVerified {
        return false, errors.New("student not verified")
    }
    
    // Check CPI cutoff
    if student.CPI < cpiEligibility {
        return false, errors.New("cpi cutoff doesnot match")
    }
    
    // Check eligibility matrix
    // Student is eligible if EITHER primary OR secondary program-department is eligible
    if eligibility[primaryID] != '1' && eligibility[secondaryID] != '1' {
        return false, errors.New("student branch not eligible")
    }
    
    return true, nil
}
```

**Key Logic**:
- Student is eligible if **EITHER** primary **OR** secondary program-department has `'1'` in eligibility matrix
- Uses direct string indexing: `eligibility[primaryID]` and `eligibility[secondaryID]`

### Database Query for Eligible Proformas

**File**: `ras-backend/application/db.proforma.go` (lines 200-239)

```go
func fetchProformaForEligibleStudent(ctx *gin.Context, rid uint, student *rc.StudentRecruitmentCycle, jps *[]Proforma) error {
    // Base query: approved proformas with deadline not passed, CPI cutoff met, not already applied
    tx := db.WithContext(ctx).
        Where("recruitment_cycle_id = ? AND is_approved = ? AND deadline > ? AND cpi_cutoff <= ? AND id NOT IN (?)",
            rid, true, time.Now().UnixMilli(), student.CPI, subQuery)
    
    // Build eligibility condition
    // SQL SUBSTRING is 1-indexed, so add 1 to ProgramDepartmentID
    eligibilityClauses = append(eligibilityClauses, "SUBSTRING(eligibility FROM ? FOR 1) = '1'")
    eligibilityArgs = append(eligibilityArgs, student.ProgramDepartmentID+1)
    
    eligibilityClauses = append(eligibilityClauses, "SUBSTRING(eligibility FROM ? FOR 1) = '1'")
    eligibilityArgs = append(eligibilityArgs, student.SecondaryProgramDepartmentID+1)
    
    // OR condition: eligible if EITHER primary OR secondary matches
    eligibilityCondition := strings.Join(eligibilityClauses, " OR ")
    tx = tx.Where(fmt.Sprintf("(%s)", eligibilityCondition), eligibilityArgs...)
    
    return tx.Find(jps).Error
}
```

**SQL Generated**:
```sql
SELECT * FROM proforma
WHERE recruitment_cycle_id = ?
  AND is_approved = true
  AND deadline > ?
  AND cpi_cutoff <= ?
  AND id NOT IN (...)
  AND (
    SUBSTRING(eligibility FROM ? FOR 1) = '1'  -- Primary program-department
    OR
    SUBSTRING(eligibility FROM ? FOR 1) = '1'  -- Secondary program-department
  )
```

**Note**: SQL `SUBSTRING` is 1-indexed, so `ProgramDepartmentID+1` is used (e.g., ID 5 → position 6 in SQL, but index 5 in Go string).

---

## Complete Flow Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                    COMPANY CREATES PROFORMA                      │
└────────────────────────────┬────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│  Step 3: Eligibility Matrix Selection (Frontend)                │
│  - Company views matrix table (27 departments × 14 programs)    │
│  - Selects eligible combinations via checkboxes                 │
│  - Can use quick-select buttons (JEE, GATE, etc.)               │
│  - State: eligibility string (130 chars)                        │
└────────────────────────────┬────────────────────────────────────┘
                              │
                              │ PUT /api/company/application/rc/:rid/proforma
                              │ { eligibility: "010101..." }
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│  Backend: putProformaByCompanyHandler()                         │
│  - Validates company authorization                              │
│  - Calls updateProformaForCompany()                            │
└────────────────────────────┬────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│  BeforeUpdate Hook: Sets index 0 to '0'                        │
└────────────────────────────┬────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│  Database: UPDATE proforma SET eligibility = '010101...'        │
│  WHERE id = ? AND company_recruitment_cycle_id = ?              │
└────────────────────────────┬────────────────────────────────────┘
                              │
                              │ (Later: Student views openings)
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│  Student: GET /api/student/application/rc/:rid/opening          │
└────────────────────────────┬────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│  Backend: getProformasForEligibleStudentHandler()               │
│  - Gets student's ProgramDepartmentID (e.g., 5 for BT-CSE)     │
│  - Gets student's SecondaryProgramDepartmentID (if any)         │
└────────────────────────────┬────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│  fetchProformaForEligibleStudent()                              │
│  - Queries proforma table                                       │
│  - Filters: is_approved=true, deadline>now, CPI>=cutoff         │
│  - Eligibility check:                                          │
│    SUBSTRING(eligibility FROM ProgramDepartmentID+1 FOR 1)='1' │
│    OR                                                           │
│    SUBSTRING(eligibility FROM SecondaryProgramDepartmentID+1)='1'│
└────────────────────────────┬────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│  Returns: List of eligible proformas                            │
│  Student sees only openings they can apply to                   │
└─────────────────────────────────────────────────────────────────┘
                              │
                              │ (Student applies)
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│  POST /api/student/application/rc/:rid/opening/:pid           │
│  - Calls GetStudentEligible() to double-check                    │
│  - Verifies: verification status, CPI, eligibility matrix        │
└─────────────────────────────────────────────────────────────────┘
```

---

## Code References

### Frontend Files

1. **Matrix Selection UI**
   - `ras-frontend/pages/company/rc/[rcid]/proforma/[proformaid]/step3.tsx`
   - Main UI for company to select eligibility

2. **Matrix Display Component**
   - `ras-frontend/components/Utils/MatrixExpanded.tsx`
   - Displays eligibility matrix as table

3. **Mapping Configuration**
   - `ras-frontend/components/Utils/matrixUtils.ts`
   - Contains `func` object mapping branch-program to string indices
   - Contains `rev` object for reverse lookup (index → program-department)

4. **API Callbacks**
   - `ras-frontend/callbacks/company/proforma.ts`
   - PUT request to update eligibility

### Backend Files

1. **Model Definition**
   - `ras-backend/application/model.go`
   - Proforma struct with `Eligibility` field

2. **Database Hooks**
   - `ras-backend/application/model.hooks.go`
   - BeforeCreate: Sets default eligibility
   - BeforeUpdate: Ensures index 0 is '0'

3. **Company Handlers**
   - `ras-backend/application/company.proforma.go`
   - PUT handler for updating proforma eligibility

4. **Database Functions**
   - `ras-backend/application/db.proforma.go`
   - `updateProformaForCompany()`: Updates eligibility in DB
   - `fetchProformaForEligibleStudent()`: Queries eligible proformas

5. **Student Eligibility Check**
   - `ras-backend/rc/db.student.go`
   - `GetStudentEligible()`: Verifies student eligibility

6. **Student Handlers**
   - `ras-backend/application/student.proforma.go`
   - `getProformasForEligibleStudentHandler()`: Returns eligible openings

7. **Router Configuration**
   - `ras-backend/application/router.go`
   - Defines API endpoints

### Utility Files

1. **Program-Department Utilities**
   - `ras-backend/util/prog_dept.go`
   - Helper functions for double major detection

---

## Key Insights

### 1. String Indexing
- **Go strings**: 0-indexed (position 5 = `eligibility[5]`)
- **SQL SUBSTRING**: 1-indexed (position 5 = `SUBSTRING(eligibility FROM 6 FOR 1)`)
- **Conversion**: SQL uses `ProgramDepartmentID + 1`

### 2. Eligibility Logic
- Student is eligible if **EITHER** primary **OR** secondary program-department is eligible
- This allows dual/double major students to apply if either program qualifies

### 3. Default Behavior
- New proformas start with **no eligibility** (all zeros)
- Company must explicitly select eligible combinations
- Index 0 is always `'0'` (reserved/unused)

### 4. Matrix Size
- Current: 130 characters (supports up to 139 program-department combinations)
- Some positions may be unused (e.g., invalid combinations)

### 5. Performance Considerations
- String indexing is O(1) - very fast
- SQL SUBSTRING queries are efficient with proper indexing
- Eligibility check happens at query time (no separate verification step needed)

---

## Example: BT-CSE Eligibility

### Scenario
Company wants to allow **BT-CSE** students to apply.

### Step-by-Step

1. **Company selects checkbox**:
   - Branch: "Computer Science and Engineering (CSE)"
   - Program: "BT" (Bachelor of Technology)

2. **Frontend calculates index**:
   ```typescript
   // From matrixUtils.ts
   func["Computer Science and Engineering (CSE)"]["BT"] = 5
   ```

3. **Frontend updates string**:
   ```typescript
   str = "000000..."  // Before
   str[5] = "1"       // Set position 5 to '1'
   str = "0000010..." // After
   ```

4. **Frontend sends to backend**:
   ```json
   PUT /api/company/application/rc/1/proforma
   {
     "ID": 123,
     "eligibility": "0000010000..."
   }
   ```

5. **Backend processes**:
   - BeforeUpdate hook: Sets index 0 to '0' (if needed)
   - Updates database: `UPDATE proforma SET eligibility = '0000010000...' WHERE id = 123`

6. **Student views openings**:
   - Student has `ProgramDepartmentID = 5` (BT-CSE)
   - Query checks: `SUBSTRING(eligibility FROM 6 FOR 1) = '1'`
   - Position 5 (index 5) = '1' → **ELIGIBLE** ✓

7. **Student applies**:
   - `GetStudentEligible()` checks `eligibility[5] == '1'`
   - Returns `true` → Application proceeds

---

## Conclusion

The Eligibility Matrix system provides a flexible and efficient way for companies to specify which program-department combinations are eligible for their job openings. The binary string representation allows for fast lookups and efficient database queries, while the frontend UI makes it easy for companies to configure eligibility through an intuitive matrix interface.

The system handles edge cases such as dual/double major students by checking both primary and secondary program-department IDs, ensuring fair access to opportunities while maintaining performance through optimized string indexing and SQL queries.
