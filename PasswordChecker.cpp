
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cctype>

using namespace std;

const string critrion_msg_length = "The selected password must be at least eight characters long.";
const string critrion_msg_upper = "The selected password must have at least one uppercase letter.";
const string critrion_msg_lower = "The selected password must have at least one lowercase letter.";
const string critrion_msg_digit = "The selected password must have at least one digit.";
const string critrion_msg_special_chars = "The selected password must have at least one of special characters: [@,!,#,^,&,*,$]";
const string critrion_msg_dictionary = "The selected password cannot contain a dictionary word.";
const string critrion_msg_leaked = "The selected password has been leaked.";

//Use this enum for check the characters of the password
const int CHECKS_NUM = 5;
enum check_names {CHECK_LENGTH, CHECK_UPPER, CHECK_LOWER, CHECK_DIGIT, CHECK_SPECIAL_CHAR};



string new_word(string word){
	string new_word;
	for (char ch : word){
		if (isalnum(ch)) {  // Check if character is alphanumeric
            if (isupper(ch)) {
                new_word += tolower(ch);  // Convert uppercase to lowercase
            }
			else {
                new_word += ch;
			}	
		}
	}
	return new_word;
}




//This functions reads in string in the file specified by the filename and fill word_dic dictionary. It going to be used to read both leaked passwords and word dictionary.
//Change any call type of parameters at your discretion (call by value or call by refrence); Do not change the name of the function.
int readWordList(string filename, vector<string>& word_dic) {
	ifstream input_file(filename);//input file stream 
	if (!input_file.is_open()) { // checks if the file has been opened successfully
		cout << "Error is opening file\n";					 
		return 1;
	}
	string line;
	while(getline(input_file,line)){
		word_dic.push_back(line);
	}
	input_file.close();
	return -1;	
}


int compare(string word, string query) {
	string mod_word = new_word(word);
	string mod_query = new_word(query);	
	if (mod_word == mod_query){
		return 0;
	}
	else if (mod_word < mod_query){
		return -1;
	}
	else {
		return 1;
	}
}

//for each rule if the passwords satisfy the rule set the corresponding entry in the checks array to true
void checkPasswordChars(bool checks[CHECKS_NUM], string password) {
	int upper = 0, lower = 0, digit = 0, special = 0;
	if(password.size() >= 8){
		checks[CHECK_LENGTH] = true;
	}
	for(size_t i = 0; i < password.size(); i++){
		char curr_key = password.at(i);
		if(isupper(curr_key) && !upper){
			checks[CHECK_UPPER] = true;
			upper++;
		}
		if(islower(curr_key) && !lower){
			checks[CHECK_LOWER] = true;
			lower++;
		}
		if(isdigit(curr_key) && !digit){
			checks[CHECK_DIGIT] = true;
		}
		if((curr_key == '@' || curr_key == '!' || curr_key == '#' || curr_key == '^' || curr_key == '*' || curr_key == '$' || curr_key == '&') && !special){
			checks[CHECK_SPECIAL_CHAR] = true;
			special++;
		}
	}
}

//This function checks if the passowrd is among the leaked password provided by password dic
//Change any call type of parameters at your discretion (call by value or call by refrence); Do not change the name of the function.
bool isInLeakedDic(vector<string>& password_dic, string password){
	int left = 0;
	int right = password_dic.size() - 1;
	int mid;
	while(left != right){
		mid = (left + right)/2;
		int match = compare(password,password_dic.at(mid));
		if (match == 0){
			return true;
		}
		else if (match == 1){
			left = mid + 1;
		}
		else if (match == -1){
			right = mid - 1;
		}
	}
	return false;
}


//This function checks if the password contains a dictionary word provided by english_dic
//Change any call type of parameters at your discretion (call by value or call by refrence); Do not change the name of the function.
bool isInEnglishDic(vector<string> english_dic, string password) {
	string mod_pass = new_word(password);
	size_t dict_mid = english_dic.size()/2;
	if (mod_pass > english_dic.at(dict_mid)){
		for (size_t i = dict_mid+1; i < english_dic.size(); i++){
				if (mod_pass.find(english_dic.at(i)) != string::npos){
					return true;
				}
			}
		}
	else if(mod_pass <= english_dic.at(dict_mid)){
		for (size_t i = 0; i <= dict_mid; i++){
				if (mod_pass.find(english_dic.at(i)) != string::npos){
					return true;
				}
			}
		}
	return false;
}


/* do not change the main file */
int main(int argc, char* argv[]) {

	if (argc < 3){
		cout << "Usage: program-name path-to-leaked-password-file path-to-english-word-file" << endl;
		return 1;
	}
	string leaked_password_file = argv[1]; 
	string english_word_file = argv[2]; 
	
	string password;


	vector<string> leaked_password_dic;
	vector<string> english_dic;

	clock_t  start = clock();
	readWordList(leaked_password_file, leaked_password_dic);
	//cout << "leaked password size: " << leaked_password_dic.size() << endl;
	
	readWordList(english_word_file, english_dic);
	//cout << "english word size: " << english_dic.size() << endl;
	
	clock_t stop = clock();

	cout << "Loading data files in " << (double)(stop - start) / CLOCKS_PER_SEC * 1000 << " milliseconds" << endl;

	cout << "Enter password: ";
	cin >> password;

	bool checks[CHECKS_NUM];

	checkPasswordChars(checks, password);


	bool failed = false;
	if (!checks[CHECK_LENGTH]) {
		failed = true;
		cout << critrion_msg_length << endl;
	}
	if (!checks[CHECK_UPPER]) {
		failed = true;
		cout << critrion_msg_upper << endl;
	}

	if (!checks[CHECK_LOWER]) {
		failed = true;
		cout << critrion_msg_lower << endl;
	}

	if (!checks[CHECK_DIGIT]) {
		failed = true;
		cout << critrion_msg_digit << endl;
	}

	if (!checks[CHECK_SPECIAL_CHAR]) {
		failed = true;
		cout << critrion_msg_special_chars << endl;
	}

	bool is_leaked = false;
	bool is_english = true;
	if (!failed) {
		start = clock();
		is_english = isInEnglishDic(english_dic, password);
		stop = clock();

		cout << "Checking against dictionary words took " <<  (double)(stop - start) / CLOCKS_PER_SEC * 1000 << " milliseconds" << endl;
		if (is_english) {
			cout << critrion_msg_dictionary << endl;
			is_english = true;
		}
		
		start = clock();
		is_leaked = isInLeakedDic(leaked_password_dic, password);
	    	stop = clock();
	    	
		cout << "Checking against leaked passwords took " <<  (double)(stop - start) / CLOCKS_PER_SEC * 1000 << " milliseconds" << endl;		
		if (is_leaked) {
			cout << critrion_msg_leaked << endl;
		}

	}
	if (!failed && !is_leaked && !is_english) {
		cout << "Password accepted!" << endl;
	}
	else {
		cout << "Password rejected!" << endl;
	}

	
	return 0;	
}
