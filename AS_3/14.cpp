#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> res;
    sort(nums.begin(), nums.end());

    for (int i = 0; i < nums.size(); i++) {
        if (i > 0 && nums[i] == nums[i-1]) continue;  // avoid duplicates
        int l = i + 1;
        int r = nums.size() - 1;

        while (l < r) {
            int sum = nums[i] + nums[l] + nums[r];

            if (sum == 0) {
                res.push_back({nums[i], nums[l], nums[r]});
                l++; r--;

                while (l < r && nums[l] == nums[l-1]) l++;  
                while (l < r && nums[r] == nums[r+1]) r--;  

            } else if (sum < 0) {
                l++;
            } else {
                r--;
            }
        }
    }
    return res;
}

int main() {
    vector<int> nums = {-1,0,1,2,-1,-4};
    auto ans = threeSum(nums);

    for (auto &v : ans)
        cout << v[0] << " " << v[1] << " " << v[2] << endl;
}
