#define FILE_EXTENSION ".txt"
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

const int alpha_num = 26;

// trie start
struct trie_node
{
	trie_node *child[alpha_num];
	// bool EOW;
	set<int> filenum;
};

trie_node *getnode(void)
{
	trie_node *tmpnode = new trie_node;

	// tmpnode->EOW = false;

	for (int i = 0; i < alpha_num; i++)
	{
		tmpnode->child[i] = NULL;
	}

	return tmpnode;
};

void insert(trie_node *root, string key,int file)
{
	trie_node *cur = root;

	for (int i = 0; i < key.length(); i++)
	{
		int index = tolower(key[i]) - 'a';
		if (!cur->child[index])
		{
			cur->child[index] = getnode();
		}

		cur = cur->child[index];
	}

	// cur->EOW = true;
	cur->filenum.insert(file);
}

set<int> exactsearch(trie_node *root, string key)
{
	trie_node *cur = root;

	for (int i = 0; i < key.length(); i++)
	{
		int idx = key[i] - 'a';
		if (!cur->child[idx])
			return {};

		cur = cur->child[idx];
	}

	return cur->filenum;
}

set<int> prefixsearch(trie_node *root, string key)
{
	trie_node *cur = root;
	set<int> trueans;

	for (int i = 0; i < key.length(); i++)
	{
		int idx = key[i] - 'a';
		if (!cur->child[idx])
			return {};

		cur = cur->child[idx];
	}

	queue<trie_node*> q;
	q.push(cur);
	while(q.size())
	{
		cur = q.front();
		q.pop();
		for(auto i:cur->filenum)
			trueans.insert(i);
		for (int i = 0; i < alpha_num; i++)
		{
			if(cur->child[i] != NULL) q.push(cur->child[i]);
		}
		
	}

	return trueans;
}
// trie end

// suffix tree start
struct suffixtree_node
{
	map<char, suffixtree_node *> child;
	suffixtree_node *link;
	bool end;
	set<int> filenum_suf;
};

suffixtree_node *start;

suffixtree_node *sfn()
{
	suffixtree_node *tmp = new suffixtree_node;
	start = tmp;
	tmp->link = tmp;
	return tmp;
}

void suffixinsert(suffixtree_node *root, char c)
{
	suffixtree_node *cur = start;
	suffixtree_node *pre;
	c = tolower(c);
	while (cur != root)
	{
		if (cur == start)
		{
			if (cur->child[c] == NULL)
			{
				suffixtree_node *tmpnode = new suffixtree_node;
				cur->end = false;
				cur->child[c] = tmpnode;
				cur->child[c]->end = true;

				cur->child[c]->link = cur->link;

				start = tmpnode;
				pre = tmpnode;
				cur = tmpnode->link;
			}
			else
			{
				cur->end = false;
				cur->child[c]->end = true;

				cur->child[c]->link = cur->link;

				start = cur->child[c];
				pre = cur->child[c];
				cur = cur->child[c]->link;
			}
		}
		else
		{
			if (cur->child[c] == NULL)
			{
				suffixtree_node *tmpnode = new suffixtree_node;
				cur->child[c] = tmpnode;
				cur->end = false;
				tmpnode->end = true;
				tmpnode->link = cur->link;

				pre->link = tmpnode;
				pre = tmpnode;
				cur = tmpnode->link;
			}
			else
			{
				cur->end = false;
				cur->child[c]->end = true;
				cur->child[c]->link = cur->link;
				pre->link = cur->child[c];
				pre = cur->child[c];
				cur = cur->child[c]->link;
			}
		}
	}

	if (cur == start)
	{
		if (cur->child[c] == NULL)
		{
			suffixtree_node *tmpnode = new suffixtree_node;
			cur->end = false;
			cur->child[c] = tmpnode;
			cur->child[c]->end = true;

			cur->child[c]->link = cur->link;

			start = tmpnode;
			pre = tmpnode;
			cur = tmpnode->link;
		}
		else
		{
			cur->end = false;
			cur->child[c]->end = true;

			cur->child[c]->link = cur->link;

			start = cur->child[c];
			pre = cur->child[c];
			cur = cur->child[c]->link;
		}
	}
	else
	{
		if (cur->child[c] == NULL)
		{
			suffixtree_node *tmpnode = new suffixtree_node;
			cur->child[c] = tmpnode;
			cur->end = false;
			tmpnode->end = true;
			tmpnode->link = cur->link;

			pre->link = tmpnode;
			pre = tmpnode;
			cur = tmpnode->link;
		}
		else
		{
			cur->end = false;
			cur->child[c]->end = true;
			cur->child[c]->link = cur->link;
			pre->link = cur->child[c];
			pre = cur->child[c];
			cur = cur->child[c]->link;
		}
	}
}

set<int> suffixsearch(suffixtree_node *root, string key)
{
	suffixtree_node *cur = root;

	for (int i = 0; i < key.length(); i++)
	{
		int idx = key[i];
		if (!cur->child[idx])
			return {};

		cur = cur->child[idx];
	}

	return cur->filenum_suf;
}

void correct(suffixtree_node *root,int file)
{
	suffixtree_node *cur = start;
	while (cur != root)
	{
		cur->filenum_suf.insert(file);
		cur = cur->link;
	}
	start = root;
}
// suffix tree end

// Utility Func

// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string)
{
	vector<string> parse_string;
	for (auto &word : tmp_string)
	{
		string new_str;
		for (auto &ch : word)
		{
			if (isalpha(ch))
				new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string &str, const string &delim)
{
	vector<string> res;
	if ("" == str)
		return res;
	// 先將要切割的字串從string型別轉換為char*型別
	char *strs = new char[str.length() + 1]; // 不要忘了
	strcpy(strs, str.c_str());

	char *d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while (p)
	{
		string s = p;	  // 分割得到的字串轉換為string型別
		res.push_back(s); // 存入結果陣列
		p = strtok(NULL, d);
	}

	return res;
}

int main(int argc, char *argv[])
{

	// INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route

	string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);

	trie_node *word1 = getnode();	// trie
	suffixtree_node *word2 = sfn(); // suffix

	ofstream of;

	int textnum = 0;
	// Read File & Parser Example

	string file, title_name, tmp;
	fstream fi;
	vector<string> tmp_string;
	vector<string> title;

	// from data_dir get file ....
	// eg : use 0.txt in data directory
	// fi.open("data/0.txt", ios::in);

	// read file
	while (1)
	{
		fi.open(data_dir + to_string(textnum) + ".txt", ios::in);
		if (fi.fail())
			break;

		getline(fi, title_name);
		// tmp_string = split(title_name, " ");
		// title.push_back(title_name);
		title.push_back(title_name);

		while (getline(fi, tmp))
		{

			// GET CONTENT WORD VECTOR
			tmp_string = split(tmp, " ");

			// PARSE CONTENT
			vector<string> content = word_parse(tmp_string);

			for (auto &word : content)
			{
				insert(word1, word, textnum);

				int len = word.length();
				for (int i = 0; i < len; i++)
				{
					suffixinsert(word2, word[i]);
				}

				correct(word2,textnum);
				// cout << word << endl;
			}
			//......
		}
		fi.close();
		textnum++;
	}

	// GET TITLENAME
	// getline(fi, title_name);

	// // GET TITLENAME WORD ARRAY
	// tmp_string = split(title_name, " ");

	// vector<string> title = word_parse(tmp_string);

	// // for(auto &word : title){
	// // 	cout << word << endl;
	// // }

	// // GET CONTENT LINE BY LINE
	// while (getline(fi, tmp))
	// {

	// 	// GET CONTENT WORD VECTOR
	// 	tmp_string = split(tmp, " ");

	// 	// PARSE CONTENT
	// 	vector<string> content = word_parse(tmp_string);

	// 	// test
	// 	for (auto &word : content)
	// 	{
	// 		insert(word1, word);

	// 		int len = word.length();
	// 		for (int i = 0; i < len; i++)
	// 		{
	// 			suffixinsert(word2, word[i]);
	// 		}

	// 		correct(word2);
	// 		// cout << word << endl;
	// 	}
	// 	//......
	// }

	// CLOSE FILE
	// fi.close();

	// test
	//  while (1)
	//  {
	//  	string text;

	// 	cin >> text;

	// 	if (text == "exit")
	// 		break;
	// 	else
	// 	{
	// 		cout << exactsearch(word1, text) << '\n';
	// 		cout << prefixsearch(word1, text) << '\n';
	// 		cout << suffixsearch(word2, text) << '\n';
	// 	}
	// }

	// search and write
	fi.open(query, ios::in);

	of.open(output);
	if (!of.is_open())
		cout << "open fail\n";

	string text;
	while (getline(fi, text))
	{
		
	}
}

// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
