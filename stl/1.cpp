#include<bits/stdc++.h>

void vectors();

int main(){
    //pair 
    
    std::pair<int , int> p = {1,3};
    std::pair<int, std::pair<int, int>> pair2 = {1, {2, 3}};
    std::pair<int, int> arr[] = {{1,2}, {2,3}};
    
    
    // std::cout << p.first << " " << p.second << "\n";
    // std::cout << pair2.first << " " << pair2.second.first << " " << pair2.second.second << "\n"; 


    vectors();

    return 0;
}

void vectors(){
    std::vector<int> v;
    v.push_back(1);
    v.emplace_back(2);
    std::cout << v[1] << "\n";
    
    std::vector<std::pair<int, int>> vecPair;
    vecPair.emplace_back(1,2);
    vecPair.push_back({5,6});
    
    //conatiner where the elements are sort of defined
    std::vector<int> v3(3, 100);
    std::cout << v3[1] << " " << v3[0] << " "  << v3[2] << "\n";

    std::vector<int> v2(v3);
    std::cout << v2[1] << " " << v2[0] << " "  << v2[2] << "\n";
    
    std::vector<int> v5;
    v5.emplace_back(6);
    v5.emplace_back(7);
    v5.emplace_back(8);


    
    std::vector<int>::iterator it2 = v5.end();
    std::cout << *(--it2) << "\n";

    
    std::cout << v5.at(1) << "\n" << "this is v of back " << v5.back() << "\n";
    std::vector<int>::iterator it = v5.begin();
    std::cout << (*it) << "\n" << *(it+2) <<"\n";

    
    std::cout << "\n Using the for loop \n";
     //printing the vector with the for loop
    for(std::vector<int>::iterator it = v5.begin(); it != v5.end() ; it++){
        std::cout << &it << " -> " << *(it) << "\n";
    }

    auto s = "hello world";
    std::cout << s << std::endl;

    
    

}