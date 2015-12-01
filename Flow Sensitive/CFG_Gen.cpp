/*
Author : lifecodemohit 
*/

#include <bits/stdc++.h>
using namespace std;

struct node {
	string block;
};

vector < string > succs_Block_Adj[1010];
vector < string > block_Lines[1010];
vector < string > block_Pointer_Stmt[1010];

static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

int extract_Block_Num(string str) {
	int value = 0;
	for (int i = 2; str[i] != ' ' && str[i] != ']'; i++) {		// [B skipped
		value = value*10 + (str[i] - '0');
	}
	return value;
}

void extract_Succs_Block(string str, int last_Block) {
	int find_colon = str.find(":");
	string result = str.substr(find_colon + 2, str.length());
	result += " ";
	for (int i = 0; i < result.length();) {
		int space_pos = result.find(" ", i);
		succs_Block_Adj[last_Block].push_back(result.substr(i, space_pos));
		// cout << result.substr(i, space_pos) << endl;
		i = space_pos + 1;
	}
}

int extract_Stmt_Num(string str) {
	int value = 0;
	for (int i = 0; i < (int)str.length(); i++) {
		if (((str[i] - '0') >= 0) && ((str[i] - '0') <= 9))
			value = value*10 + (int)(str[i] - '0');
		else
			break;
	}
	return value;
}

int extract_Dot_Num(string str) {
	int value = 0;
	for (int i = 0; str[i] != ' ' && str[i] != ']'; i++) {		// [B skipped
		value = value*10 + (str[i] - '0');
	}
	return value;
}

int main() {
	vector < string > lines;
	struct node * root;

	getchar();
	string str;
	while (getline(cin, str)) {
		// cout << str << endl;
		str = trim(str);
		lines.push_back(str);
	}

	int lines_sz = lines.size();
	
	// cout << lines[0] << endl;	// Virtual Entry
	int last_Block = extract_Block_Num(lines[0]);
	//cout << last_Block << endl;
	int num_Block = last_Block;

	for (int i = 0; i < lines_sz; i++) {
		if ((lines[i].length() == 0) && (i + 1 < lines_sz)) {
			// cout << lines[i+1] << endl;
			last_Block = extract_Block_Num(lines[i+1]);
		}
		if (lines[i].find("Succs") != string::npos) {
			// cout << lines[i] << endl;
			extract_Succs_Block(lines[i], last_Block);
		}
		
		if ((lines[i].length() != 0) && (lines[i].find("Succs") == string::npos) && (lines[i].find("Preds") == string::npos)) {
			block_Lines[last_Block].push_back(lines[i]);
			// cout << lines[i] << " " << last_Block << endl;
			// cout << block_Lines[last_Block].size() << endl;
		}
		num_Block = max(num_Block, last_Block);
	} 

	// Handling blocks are individually
	for (int i = 0; i <= num_Block; i++) {
		map < int, string > map_Block_Stmt;
		for (int j = 1; j < (int)block_Lines[i].size(); j++) {
			// cout << block_Lines[i][j] << endl;
			int stmt_No = extract_Stmt_Num(block_Lines[i][j]);
			int find_colon = block_Lines[i][j].find(":");
			map_Block_Stmt[stmt_No] = block_Lines[i][j].substr(find_colon+2, block_Lines[i][j].length());
			string stmt = block_Lines[i][j].substr(find_colon+2, block_Lines[i][j].length());
			if (stmt.find(" = ") != string::npos) {
				stringstream ss;
				ss << i;
				int check_stmt = stmt.find("[B" + ss.str() + ".");
				while (check_stmt != string::npos) { 
					int stmt_num = extract_Dot_Num(stmt.substr(check_stmt + block_Lines[i][0].length(), stmt.length()));	
					stmt =  stmt.substr(0, check_stmt) + map_Block_Stmt[stmt_num] + stmt.substr(stmt.find("]") + 1, stmt.length());
					// cout << stmt.substr(0, check_stmt) + map[stmt_num] + stmt.substr(check_stmt + block_Lines[i][0].length() + 1 + 1 + 2);
					check_stmt = stmt.find("[B" + ss.str() + ".");
				}
				// cout << stmt << endl;
				block_Pointer_Stmt[i].push_back(stmt);
			}
		}
	}

	// for (int  i = 0; i <= num_Block; i++) {
	// 	cout << "Block Number : " << i << endl;
	// 	cout << "Size of Block : " << (int)block_Pointer_Stmt[i].size() << endl; 
	// 	for (int j = 0; j < (int)block_Pointer_Stmt[i].size(); j++) {
	// 		cout << block_Pointer_Stmt[i][j] << endl;
	// 	}
	// 	cout << endl;
	// }

	// cout << endl;

	// cout << "Number of blocks : " << num_Block << endl;
	// for (int i = 0; i <= num_Block; i++) {
	// 	cout << "Block number " << i << " successor : ";
	// 	for (int j = 0; j < (int)succs_Block_Adj[i].size(); j++) {
	// 		cout << succs_Block_Adj[i][j] << " ";
	// 	}
	// 	cout << endl;
	// } 

	cout << num_Block << endl;
	for (int  i = 0; i <= num_Block; i++) {
		cout << succs_Block_Adj[i].size() << endl;
		for (int j = 0; j < (int)succs_Block_Adj[i].size(); j++) {
			cout << succs_Block_Adj[i][j].substr(1, succs_Block_Adj[i][j].length()) << " ";
		}
		cout << endl;
	}

	for (int  i = 0; i <= num_Block; i++) {
		cout << block_Pointer_Stmt[i].size() << endl;
		for (int j = 0; j < (int)block_Pointer_Stmt[i].size(); j++) {
			cout << block_Pointer_Stmt[i][j] << endl;
		}
	}

	return 0;
}