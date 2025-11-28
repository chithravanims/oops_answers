#include <bits/stdc++.h>
using namespace std;

bool isValid(string s) {
    stack<char> st;
    unordered_map<char, char> match = {
        {')','('}, {'}','{'}, {']','['}
    };

    for (char c : s) {
        if (c == '(' || c=='{' || c=='[')
            st.push(c);
        else {
            if (st.empty() || st.top() != match[c])
                return false;
            st.pop();
        }
    }
    return st.empty();
}

int main() {
    string s = "{[()]}";
    cout << (isValid(s) ? "Valid" : "Invalid");
}
