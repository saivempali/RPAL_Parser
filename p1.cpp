/*
*p1.cpp
*-------------------------------
* Author : Sai Vempali
* UFID   : 16141381
* email  : vishnu24@ufl.edu
*-------------------------------
*/


#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <cstdlib>
#include <stack>
#include <vector>

using namespace std;

//=====================TOKEN CLASS=======================
class Token {
public:
	Token(){}
	Token(string,string type);
	string value;
	string type;

};

Token::Token(string value,string type){
	this->value = value;
	this->type = type;
}

//=====================TREE NODE CLASS=======================
class TreeNode {
public:
	TreeNode();
	
	Token value;
	TreeNode* right;
	TreeNode* left;
};

TreeNode::TreeNode() {
	right = NULL;
	left = NULL;
}

//=====================RECURSIVE DESCENT PARSER CLASS=======================
class RDP
{
	private:
		
		string program;
		int size;
		int tokensPtr;
		bool isOperator(char);
		bool isKeyword(string);
		bool tokensLeft;
		vector<Token> tokens;
		stack<TreeNode*> parseStack;
		Token currentValue;
		
		
	public:
		RDP(string);
		RDP();
		void PreorderTraversal(TreeNode*,string);
		void reader(Token);
		void TreeBuilder(Token,int);
		void E();
		void Ew();
		void T();
		void Ta();
		void Tc();
		void B();
		void Bt();
		void Bs();
		void Bp();
		void A();
		void At();
		void Af();
		void Ap();
		void R();
		void Rn();
		void D();
		void Da();
		void Dr();
		void Db();
		void Vb();
		void V1();
		Token getNextToken();
		Token nextCheck();
		void generateTokens(string);
		void printTree();
		void startParsing();
};

RDP::RDP(string program) {
	this->program = program;
	this->size = program.size();
	this->tokensPtr = 0;
	this->tokensLeft = true;
}

//=====================METHOD TO GENERATE TOKENS=======================
/*GENERATES TOKENS AND SAVES THEM IN A VECTOR*/

void RDP::generateTokens(string code)
{
	string tokval = "";
	
	for(int ptr=0; ptr<code.length(); ptr++)
	{
		if(code[ptr] == ' ' || code[ptr] == '\n' || code[ptr] == '\t')
		{
			continue;
		}
		
		if(isalpha(code[ptr]))
		{
			tokval = tokval + code[ptr];
			ptr++;

			while(code[ptr] != ' ')
			{
				if(isalpha(code[ptr]) || isdigit(code[ptr]) || code[ptr] == '_')
				{
					tokval = tokval + code[ptr];
					ptr++;
				}

				else
				{
					ptr--;
					break;
				}
			}
			if(isKeyword(tokval))
			{
				Token t(tokval,"KEYWORD");
				tokens.push_back(t);
				tokval = "";
			}
			else
			{
				Token t(tokval,"IDENTIFIER");
				tokens.push_back(t);
				tokval = "";
			}
		}

		else if(isdigit(code[ptr]))
		{
			
			tokval +=code[ptr];
			ptr++;

			while(code[ptr] != ' ')
			{
				if(isdigit(code[ptr]))
				{
					tokval += code[ptr];
					ptr++;
				}
				else
				{
					ptr--;
					break;
				}
			}
				Token t(tokval, "INTEGER");
				tokens.push_back(t);
				tokval = "";
			
		}

		else if(code[ptr] == '\'')
		{
			
 			tokval += code[ptr];
			
			ptr++;
			while(true)
			{
				
				char ch = code.at(ptr);
				ptr++;
				if(ch == '\\')
				{
					
					char ch1 = code.at(ptr);
					ptr++;
					if(ch1 =='t' || ch1 == 'n' || ch1=='\\' || ch1=='\'')
					{
						tokval += ch;
						tokval += ch1;
					}
					else
					{
						throw "Error: STRING not created";
					}
					
				}
				else if(ch == '\'')
				{
					
					tokval +=ch;
					
					Token t(tokval,"STRING");
					tokens.push_back(t);
					tokval = "";
					break;
				} 
				else if(isalpha(ch) || isdigit(ch) || isOperator(ch) || ch==')' || ch=='(' || ch==';' || ch==','
						|| isspace(ch))
				{
					
					tokval +=ch;
					
				}
			}
			char ch = code[ptr];
			if(isalpha(ch) || isdigit(ch) || isOperator(ch) || ch==')' || ch=='(' || ch==';' || ch==','
						|| isspace(ch))
			{
				tokval +=ch;
				Token t(tokval,tokval);
					tokens.push_back(t);
					tokval = "";
			}
		}
		else if(isOperator(code[ptr]))
		{
			if(code[ptr] == '-')
			{
				if(code[ptr+1] == '>')	
				{
					string st = "->";
					Token t(st,"OPERATOR");
					tokens.push_back(t);
					ptr++;
				}
				else
				{
					string st = "";
					st = st + code[ptr];
					Token t(st,"OPERATOR");
					tokens.push_back(t);
				}
			}

			else if(code[ptr] == '>' || code[ptr] == '<')
			{
				if(code[ptr+1] == '=')
				{
					string st = "";
					st = st + code[ptr] + code[ptr+1];
					Token t(st,"OPERATOR");
					tokens.push_back(t);
					ptr++;
				}
				else
				{
					string st = "";
					st = st + code[ptr];
					Token t(st,"OPERATOR");
					tokens.push_back(t);
				}
			}
			else if(code[ptr] == '/')
			{
				ptr++;
				if(code[ptr] == '/')
				{
					for(;;){
						char ch = program.at(ptr++);
						if(ch == '\n'){
							ptr--;
							break;
						}else if(isalpha(ch) || isdigit(ch) || isOperator(ch) || isspace(ch) || ch=='\t'
								|| ch=='\'' || ch == '(' || ch==')' || ch==';' || ch==',' || ch=='\\'){
							continue;
						}
					}
					continue;
				}
				else
				{
					ptr--;
					string st = "";
					st += code[ptr];
					while(true){
						if(ptr != size){
							char ch = program.at(tokensPtr++);
							if(isOperator(ch)){
								st +=ch;
							}else{
								tokensPtr--;
								break;
							}
						}else{
							break;
						}
					}
					Token t(st,"OPERATOR");	
					tokens.push_back(t);
				}
			}
			
			else
			{
				string st = "";
				st = st + code[ptr];
				Token t(st,"OPERATOR");
				tokens.push_back(t);
			}
		}

		else if(code[ptr] == '(' || code[ptr] == ')' || code[ptr] == ';' || code[ptr] == ',')
		{
			string st = "";
			st += code[ptr];
			Token t(st,st);
			tokens.push_back(t);
		}

	}

	vector<Token> filteredTokens;

	for(int i=0; i<tokens.size(); i++)
	{
		if(isspace(tokens[i].value[0]) || tokens[i].value[0] == '\t' || tokens[i].value[0] == '\n' )
			continue;

		filteredTokens.push_back(tokens[i]);
	}
	
	tokens = filteredTokens;
	
}

//=====================METHOD TO RETRIEVE NEXT TOKEN FROM VECTOR FOR PARSING=======================
Token RDP::getNextToken()
{
	if(tokensPtr == tokens.size())
	{
		throw "Error: All tokens are done parsing";
	}
	Token token = tokens[tokensPtr];
	tokensPtr++;
	return token;

}

Token RDP::nextCheck()
{
	if(tokensPtr == size)
		throw "No more tokens";
	Token t = getNextToken();
	tokensPtr--;
	return t;
}

//=====================METHOD TO CHECK WHETHER OPERATOR OR NOT=======================
bool RDP::isOperator(char ch)
{
	char operators[] = {'+','-','*','<','>','&','.','@','/',':','=','-','|','$','!','#','%','^','_','[',']','{','}','"','`','?'};
	int i=0;
	while(i<26)
	{
		if(ch==operators[i])
			return true;
		i++;
	}
	return false;
}

//=====================METHOD TO CHECK WHETHER KEYWORD OR NOT=======================
bool RDP::isKeyword(string tokenValue)
{
	string keywords[] = {"let","where","aug","or","not","gr","ge","ls","le","eq","ne","true","false","nil","dummy","in","fn"
						,"within","and","rec","list"};
	int i=0;
	while(i<21)
	{
		if(tokenValue == keywords[i])
			return true;
		i++;
	}
	return false;
}

//=====================METHOD TO CREATE NODE=======================
void RDP::reader(Token token){
	
	if(tokensLeft == false)
	{
		throw "Error: No more tokens left to be parsed";
	}
	
	if(token.type == "IDENTIFIER" || token.type == "STRING" || token.type == "INTEGER")
	{
		TreeBuilder(token, 0);
	}

	try
	{
		currentValue = getNextToken();
	}

	catch(const char* msg)
	{
		Token last("#","#");
		currentValue = last;
		tokensLeft = false;
	}
}


void RDP::Ew(){
	
	T();
	if(currentValue.value == "where")
	{
		Token t("where","KEYWORD");
		reader(t);
		Dr();
		Token node("where","where");
		TreeBuilder(node,2);
	}
	
}
void RDP::E()
{	
	if(currentValue.value == "let")
	{
		Token t("let","KEYWORD");
		reader(t);
		D();
		Token t1("in","KEYWORD");
		reader(t1);
		E();
		Token node("let","let");
		TreeBuilder(node,2);
	}

	else if(currentValue.value == "fn")
	{
		int n=0;
		Token t("fn","fn");
		reader(t);
		Vb();
		n++;
		while(currentValue.type == "IDENTIFIER" || currentValue.type == "(")
		{
			Vb();
			n++;		
		}
		Token t1(".","OPERATOR");
		reader(t1);
		E();
		Token node("lambda","lambda");
		TreeBuilder(node,n+1);
	} 

	else
	{
		Ew();
	}
	
}

void RDP::A()
{
	if(currentValue.value == "-")
	{
		Token minus("-","OPERATOR");
		reader(minus);
		At();
		Token node("neg","neg");
		TreeBuilder(node,1);
	}
	
	else if(currentValue.value == "+")
	{
		Token plus("+","OPERATOR");
		reader(plus);
		At();
	}

	else
	{
		At();
	}

	for(;currentValue.value == "+" || currentValue.value == "-";)
	{
		Token t = currentValue;
		reader(currentValue);
		At();
		TreeBuilder(t,2);
	}
	
}

void RDP::At()
{	
	Af();
	for(;currentValue.value == "*" || currentValue.value=="/";)
	{
		Token t = currentValue;
		reader(currentValue);
		Af();
		TreeBuilder(t,2);
	}
	
}

void RDP::Af()
{	
	Ap();
	for(;currentValue.value == "**";)
	{
		Token t = currentValue;
		reader(currentValue);
		Af();
		TreeBuilder(t,2);
	}	
}

void RDP::Ap()
{	
	R();
	for(;currentValue.value == "@";)
	{
		Token t = currentValue;
		reader(currentValue); 
		if(currentValue.type != "IDENTIFIER")
			throw "Error in Ap()";
		reader(currentValue); 
		R();
		TreeBuilder(t,3);
	}
	
}

void RDP::B()
{	
	Bt();
	for(;currentValue.value == "or";)
	{
		Token t = currentValue;
		reader(currentValue);
		Bt();
		TreeBuilder(t,2);
	}
	
}

void RDP::Bt()
{	
	Bs();
	for(;currentValue.value == "&";)
	{
		Token t = currentValue;	reader(currentValue);
		Bs();
		TreeBuilder(t,2);
	}
}

void RDP::Bs()
{	
	if(currentValue.value != "not")
	{
		Bp();	
	}
	else
	{
		Token t = currentValue;
		reader(currentValue);
		Bp();
		TreeBuilder(t,1);
	}
}

void RDP::Bp()
{	
	A();
	
	if(currentValue.value =="eq" )
	{
		reader(currentValue);
		A();
		Token t("eq","eq");
		TreeBuilder(t,2);
	}

	else if(currentValue.value =="ne")
	{
		reader(currentValue);
		A();
		Token t("ne","ne");
		TreeBuilder(t,2);
	}
	
	if(currentValue.value=="gr" || currentValue.value == ">")
	{
		reader(currentValue);
		A();
		Token t("gr","gr");
		TreeBuilder(t,2);
	}
	
	else if(currentValue.value == "ls" || currentValue.value == "<")
	{
		reader(currentValue);
		A();
		Token t("ls","ls");
		TreeBuilder(t,2);
	}

	else if(currentValue.value =="ge" || currentValue.value == ">=" )
	{
		reader(currentValue);
		A();
		Token t("ge","ge");
		TreeBuilder(t,2);
	}

	else if(currentValue.value =="le" || currentValue.value == "<=" )
	{
		reader(currentValue);
		A();
		Token t("le","le");
		TreeBuilder(t,2);
	}

}

void RDP::R()
{
	
	Rn();
	while(currentValue.type == "IDENTIFIER" || currentValue.type == "STRING" || currentValue.type == "INTEGER" ||currentValue.value == "true" || currentValue.value == "false" || currentValue.value == "nil" || currentValue.value == "(" || currentValue.value == "dummy")
	{
		Rn();
		Token Node("gamma","gamma");
		TreeBuilder(Node,2);
	}
	
}

void RDP::Rn()
{
	
	if(currentValue.value == "(")
	{
		reader(currentValue);
		E();
		Token t(")",")");
		reader(t);
	}

	else if(currentValue.value=="true")
	{
		reader(currentValue);
		Token Node("true","true");
		TreeBuilder(Node,0);
	}

	else if(currentValue.value == "false")
	{
		reader(currentValue);
		Token Node("false","false");
		TreeBuilder(Node,0);
	}

	else if(currentValue.value == "nil")
	{
		reader(currentValue);
		Token Node("nil","nil");
		TreeBuilder(Node,0);
	}

	else if(currentValue.value == "dummy")
	{
		reader(currentValue);
		Token Node("dummy","dummy");
		TreeBuilder(Node,0);
	}

	else if(currentValue.type == "IDENTIFIER" || currentValue.type == "STRING" || currentValue.type == "INTEGER" )
	{
		reader(currentValue);
	}
}

void RDP::T(){
	
	Ta();
	if(currentValue.value == ",")
	{
		int n = 0;
		reader(currentValue);
		Ta();
		n++;
		while(currentValue.value == ",")
		{
			reader(currentValue);
			Ta();
			n++;
		}
		Token tau("tau","tau");
		TreeBuilder(tau,n+1);
	}
	
}

void RDP::Ta()
{	
	Tc();
	for(;currentValue.value == "aug";)
	{
		Token t = currentValue;
		reader(currentValue);
		Tc();
		TreeBuilder(t,2);
	}
	
}

void RDP::Tc()
{	
	B();
	if(currentValue.value == "->")
	{
		reader(currentValue);
		Tc();
		Token t("|","OPERATOR");
		reader(t);
		Tc();
		Token node("->","->");
		TreeBuilder(node,3);
	}
	
}

void RDP::D()
{	
	Da();
	if(currentValue.value == "within")
	{
		reader(currentValue);
		D();
		Token Node("within", "within");
		TreeBuilder(Node,2);
	}	
}

void RDP::Dr()
{
	
	if(currentValue.value != "rec")
	{
		Db();
	}

	else
	{
		Token t = currentValue;
		reader(currentValue);
		Db();
		TreeBuilder(t,1);
	}
	
}

void RDP::Da()
{
	
	Dr();
	if(currentValue.value== "and")
	{
		int n = 1;
		Token t = currentValue;
		for(;currentValue.value == "and";)
		{
			reader(currentValue);
			Dr();
			n++;
		}
		TreeBuilder(t,n);
	}
}

void RDP::Db()
{
	
	if(currentValue.value == "(")
	{
		reader(currentValue);
		D();
		Token t(")","OPERATOR");
		reader(t);
	}

	else if(currentValue.type == "IDENTIFIER" && (nextCheck().value == "," || nextCheck().value == "="))
	{
		V1();
		Token t("=","OPERATOR");
		reader(t);
		E();	
		Token Node("=","=");
		TreeBuilder(Node,2);
	
	}
	else
	{
		reader(currentValue);
		int n = 1;
		Vb();
		for(;currentValue.type == "IDENTIFIER" || currentValue.value== "(";)
		{
			n++;
			Vb();
		}
		Token t("=","OPERATOR");
		reader(t);
		E();
		Token Node("function_form", "function_form");
		TreeBuilder(Node, n+2);
	}
}

void RDP::Vb()
{
	
	if(currentValue.value == "(")
	{
		reader(currentValue);
		if(currentValue.value != ")")
		{
			V1();
			Token t(")",")");
			reader(t);
		}		


		if(currentValue.value == ")")
		{
			reader(currentValue);
			Token Node("()","()");
			TreeBuilder(Node,0);
		}

		
	}

	if(currentValue.type == "IDENTIFIER")
	{
		reader(currentValue);
	}
}

void RDP::V1()
{
	
	reader(currentValue);
	int n =1;
	if(currentValue.value == ",")
	{
		for(;currentValue.value == ",";)
		{
			n++;
			reader(currentValue); 
			reader(currentValue); 
		}
		Token Node(",",",");
		TreeBuilder(Node,n);
	}
	
}

//=====================METHOD TO BUILD TREE=======================
void RDP::TreeBuilder(Token token, int n){
	
	TreeNode* node = new TreeNode;
	node->value = token;
	if(n != 0){
		int i=0;
		for(;i<n-1;)
		{			
			TreeNode* node1 = new TreeNode;
			if(parseStack.empty())
				node1 = NULL;
			else
			{
				node1 = parseStack.top();
				parseStack.pop();
			}

			parseStack.top()->right = node1;
			i++;
		}

		if(!parseStack.empty())
		{
			node->left = parseStack.top();
			parseStack.pop();
		}
		
	}
	parseStack.push(node);
}



//=====================METHOD TO PRINT TREE=======================
void RDP::printTree()
{
	
	TreeNode* t = parseStack.top();
	parseStack.pop();
	PreorderTraversal(t,string(""));
}


void RDP::PreorderTraversal(TreeNode* t, string s)
{

	Token token = t->value;

	cout<<s;

	if(token.value == "true" || token.value == "false" || token.value == "nil" || token.value== "dummy")
	{
		cout<<'<'<<token.value<<'>'<<endl;
	}
	
	else if(token.type == "IDENTIFIER")
	{
		cout<<"<ID:"<< token.value<<'>'<<endl;
	}

	else if(token.type == "INTEGER")
	{
		cout<<"<INT:"<< token.value<<'>'<<endl;
	}

	else if(token.type == "STRING")
	{
		cout<<"<STR:"<<token.value<<'>'<<endl;
	}

	else
	{
		cout<<token.value<<endl;
	}

	if(t->left != NULL)
		PreorderTraversal(t->left,("."+s));

	if(t->right != NULL)
		PreorderTraversal(t->right,(".",s));
}


void RDP::startParsing()
{
	currentValue = getNextToken();
	E();
}

int main(int argc,char *argv[]) 
{
	if(argc < 3)
		return 0;

	int i=0;
	bool showAST = false;
	while(i<argc){

		if(strcmp(argv[i],"-ast") == 0){
			showAST = true;
			break;
		}
		i++;
	}
	if(showAST == true){
		
		char *file = argv[argc-1];
		ifstream in;
		in.open(file);
		string line;
		string code = "";
		if(in.is_open())
		{
			
			while(getline(in,line))
			{
				code += line + '\n';
			}
		}
		else
			cout<<"File opening unsuccessful\n";

		RDP rp(code);
		rp.generateTokens(code);
		rp.startParsing();
		rp.printTree();
	}
	
	return 0;
}
