/*
Author : lifecodemohit 
*/

#include <bits/stdc++.h>
using namespace std;

vector < string > block_Lines[1010];
vector < int > succ_adj[1010];
vector < int > pred_adj[1010];
vector < string > output_Set[1010];

vector < string > gen_Outset(int pos, vector < string > inset) {

	vector < string > outset;
	bool flag = false;
	// cout << pos << endl;
	for (int i = 0; i < (int)block_Lines[pos].size(); i++) {
		outset.clear();
		// cout << block_Lines[pos][i] << endl;
		if (block_Lines[pos][i][0] == '*') {

		}
		else {
			if (block_Lines[pos][i].find(" = &") != string::npos) {
				flag = true;
				string left = "";
				string str = block_Lines[pos][i];
				left = str.substr(0, str.find(" "));
				string right = "";
				right = str.substr(str.find("&") + 1, str.length());
				// cout << left << "  " << right << endl;
				for (int j = 0; j < (int)inset.size(); j++) {
					if (inset[j].find(left+"->") == string::npos) {
						outset.push_back(inset[j]);
					}
				}
				outset.push_back(left + "->" + right);
			}
			else if (block_Lines[pos][i].find(" = *") != string::npos) {
				flag = true;
				string left = "";
				string str = block_Lines[pos][i];
				left = str.substr(0, str.find(" "));
				string right = "";
				right = str.substr(str.find("*") + 1, str.length());
				// cout << left << "  " << right << endl;
				for (int j = 0; j < (int)inset.size(); j++) {
					if (inset[j].find(left+"->") == string::npos) {
						outset.push_back(inset[j]);
					}
				}
				// cout << inset.size() << endl;
				for (int j = 0; j < (int)inset.size(); j++) {
					// cout << inset[j] << endl;
					if (inset[j].find(right+"->") != string::npos) {
						// cout << left + "->" + inset[j].substr(right.length() + 2, inset[j].length()) << endl;
						// cout << inset[j].substr(right.length() + 2, inset[j].length())<< endl;
						for (int k = 0; k < (int)inset.size(); k++) {
								
							if (inset[k].find(inset[j].substr(right.length() + 2, inset[j].length())+"->") != string::npos) {
								
								outset.push_back(left + "->" + inset[k].substr(inset[j].substr(right.length() + 2, inset[j].length()).length() + 2, inset[k].length()));
							}
						} 
					}
				}
			}
			else if (block_Lines[pos][i].find(" = ") != string::npos) {
				flag = true;
				string left = "";
				string str = block_Lines[pos][i];
				left = str.substr(0, str.find(" "));
				string right = "";
				right = str.substr(str.find("=") + 2, str.length());
				// cout << left << "  " << right << endl;
				for (int j = 0; j < (int)inset.size(); j++) {
					if (inset[j].find(left+"->") == string::npos) {
						outset.push_back(inset[j]);
					}
				}
				for (int j = 0; j < (int)inset.size(); j++) {
					if (inset[j].find(right+"->") != string::npos) {
						// cout << left + "->" + inset[j].substr(right.length() + 2, inset[j].length()) << endl; 
						outset.push_back(left + "->" + inset[j].substr(right.length() + 2, inset[j].length()));
					}
				}
			}
		}
		if (flag) {
			inset.clear();
			inset = outset;
		}
	}
	if (!flag)
		return inset;

	return outset;
}


void bfs(int pos, int visit[]) {

	int que[1010];
	visit[pos] = true;
	
	int front = 0;
	int rear = 0;
	que[front] = pos;
	

	while (front <= rear) {
		int top = que[front];
		map < string , int > mark;
		vector < string > inset;
		for (int i = 0; i < (int)pred_adj[top].size(); i++) {
			// cout << "Pred : " << pred_adj[top][i] <<  " " ;
			for (int j = 0; j < (int)output_Set[pred_adj[top][i]].size(); j++) {
				if (mark[output_Set[pred_adj[top][i]][j]] == 0) {
					inset.push_back(output_Set[pred_adj[top][i]][j]);
					mark[output_Set[pred_adj[top][i]][j]] = 1;
				}
			}
		}
		// cout << inset.size() << endl;
		output_Set[top] = gen_Outset(top, inset);
		
		// cout << top << endl;
		for (int i = 0; i < (int)succ_adj[top].size(); i++) {
			
			if (visit[succ_adj[top][i]] == 0) {
				// cout << succ_adj[top][i] << " " ;
				rear++;
				que[rear] = succ_adj[top][i];
				visit[succ_adj[top][i]] = 1;
			}
		}
		// cout << endl;
		front++;
	}
}

int main() {

	int num_Block;
	cin >> num_Block;
	for (int  i = 0; i <= num_Block; i++) {
		int sz_block;
		cin >> sz_block;
		for (int j = 0; j < sz_block; j++) {
			int b_no;
			cin >> b_no;
			succ_adj[i].push_back(b_no);
			pred_adj[b_no].push_back(i);
		}
	}

	for (int i = 0; i <= num_Block; i++) {
		int sz_lines;
		cin >> sz_lines;
		getchar();
		for (int j = 0; j < sz_lines; j++) {
			string stmt;
			getline(cin, stmt);
			block_Lines[i].push_back(stmt);
		}
	}

	int visit[1010];
	memset(visit, 0, sizeof(visit));
	bfs(num_Block, visit);

	for (int i = 0; i <= num_Block; i++) {
		cout << "Block : " << i << endl;
		for (int j = 0; j < (int)output_Set[i].size(); j++) {
			cout << output_Set[i][j] << endl;
		}
	}
	return 0;
}