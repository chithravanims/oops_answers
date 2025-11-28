#include <bits/stdc++.h>
using namespace std;

vector<pair<int,int>> mergeIntervals(vector<pair<int,int>>& intervals) {
    if (intervals.empty()) return {};

    sort(intervals.begin(), intervals.end());  
    vector<pair<int,int>> merged;

    merged.push_back(intervals[0]);

    for (auto &curr : intervals) {
        auto &last = merged.back();

        if (curr.first <= last.second) {
            last.second = max(last.second, curr.second); // merge
        } else {
            merged.push_back(curr); // no overlap
        }
    }
    return merged;
}

int main() {
    vector<pair<int,int>> intervals = {{1,3},{2,6},{8,10},{15,18}};
    auto ans = mergeIntervals(intervals);

    for (auto &p : ans) cout << p.first << "," << p.second << endl;
}
