#include <bits/stdc++.h>
using namespace std;

// Return list of unique pairs (a, b) whose sum = target
vector<pair<int,int>> twoSum(const vector<int>& nums, int target) {
    unordered_map<int,int> mp;   // value -> index
    set<pair<int,int>> result;   // to avoid duplicates

    for (int num : nums) {
        int diff = target - num;

        if (mp.count(diff)) {
            int a = min(num, diff);
            int b = max(num, diff);
            result.insert({a, b});
        }
        mp[num]++;  // store frequency
    }

    return vector<pair<int,int>>(result.begin(), result.end());
}

int main() {
    vector<int> nums = {2, 7, 11, 15, 7, 2};
    int target = 9;

    auto ans = twoSum(nums, target);
    for (auto &p : ans)
        cout << p.first << " " << p.second << endl;
}
