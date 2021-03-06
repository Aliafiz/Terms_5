#include<bits/stdc++.h>
using namespace std;
class Lexical{
	private:
		bool isNotes=false,isQuate=false;
		int lineNum=0;
		string str;
		//用于计数
		int wordNum=0,keyWordNum=0,symbolNum=0,intNum=0,decimalNum=0,unsignedNum=0;
		ifstream File;
		//关键词表
		map<string,bool>keyWord;
		//格式化输出
		void Cout(string str,string type){
			cout<<"line:"<<lineNum<<"{type:"<<type<<",\""<<str<<"\"}"<<endl;
		}
		void Cout(char ch,string type){
			cout<<"line:"<<lineNum<<"{type:"<<type<<",\""<<ch<<"\"}"<<endl;
		}
		bool isLetter(char ch){
			return (ch<='z'&&ch>='a')||(ch<='Z'&&ch>='A');
		}
		bool isNum(char ch){
			return ch<='9'&&ch>='0';
		}
		bool isKey(string str){
			return keyWord[str];
		}
		void error(){
			cout<<"line:"<<lineNum<<"  error"<<endl;
		}
	public:
		Lexical(){
			lineNum=0;
			//打开关键词表并读取其中信息
			File.open("keyword.txt",ios::in);
			string word;
			while(getline(File,word,'\n')){
				keyWord[word]=true;
			}
		}
		~Lexical(){
			File.close();
		}
		//输出各类词法信息
		void printInfor(){
			cout<<"line:"<<lineNum<<endl;
			cout<<"symbol:"<<symbolNum<<endl;
			cout<<"word:"<<wordNum<<endl;
			cout<<"key word:"<<keyWordNum<<endl;
			cout<<"integer:"<<intNum<<endl;
			cout<<"decimal:"<<decimalNum<<endl;
			cout<<"unsigned:"<<unsignedNum<<endl;
		}
		//检查注释或引用是否有结尾
		void check(){
			if(isQuate||isNotes)
			error();
		}
		void analysis(string s){
			lineNum++;
			s+=' ';
			int state=0;
			if(isNotes) state=12;
			if(isQuate) state=14;
		//	cout<<state<<endl;
			char ch=' ';
			auto it=s.begin();
			string ptr=str;
			str.clear();
				while(it!=s.end()){
				switch(state){
					//初始状态
					case 0:{
						ptr.clear();
						ch=*it++;
						switch(ch){
							case ' ':state=0;break;
							case '<':state=8;if(it==s.end()) Cout("<","-");break;
							case '>':state=9;if(it==s.end()) Cout(">","-");break;
							case '=':state=10;if(it==s.end()) Cout("=","-");break;
							case '/':state=11;if(it==s.end()) Cout("/","-");break;
							case '"':state=14;break;
						//	case '"':
							case '&':
							case '%':
							case '#':
							case '.':
							case ':':
							case '(':
							case ')':
							case '[':
							case ']':
							case '{':
							case '}':
							case ',':
							case ';':
							case '\'':{
								state=0;
								symbolNum++;
								Cout(ch,"-");
								break;
							}
							case '+':
							case '-':
								if(it!=s.end()&&*it==ch){
									string p;
									p+=ch;
									p+=ch;
									Cout(p,"-");
									symbolNum++;
									it++;
									break;
								}
							case '*':
								if(it!=s.end()&&*it=='='){
									Cout(ch+"=","-");
									symbolNum++;
									it++;
									break;
								}
								Cout(ch,"-");
								symbolNum++;
								break;
							default:{
								if(isLetter(ch)){
									ptr+=ch;
									state=1;
								}else if(isNum(ch)){
									ptr+=ch;
									state=2;
								}else state=13;
								break;
							}
						}
						break;
					}
					//输入字符串
					case 1:{
						ch=*it++;
						if(isLetter(ch)||isNum(ch)||ch=='_'){
							ptr+=ch;
							state=1;
						}else{
							
							it--;
							state=0;
							if(isKey(ptr)){
								keyWordNum++;
								Cout(ptr,"KeyWord");
							}else{
								wordNum++;
								Cout(ptr,"Variable");
							}
						}
						break;
					}
					case 2:{
						ch=*it++;
						if(isNum(ch)){
							ptr+=ch;
							state=2;
						}else if(ch=='.'){
							ptr+=ch;
							state=3;
						}else if(ch=='e'){
							ptr+=ch;
							state=5;
						}else{
							it--;
							state=0;
							intNum++;
							Cout(ptr,"Integer");
						}
						break;
					}
					case 3:{
						ch=*it++;
						if(isNum(ch)){
							ptr+=ch;
							state=4;
						}else{
							error();
							state=0;
						}
						break;
					}
					case 4:{
						ch=*it++;
						if(isNum(ch)){
							ptr+=ch;
							state=4;
						}else if(ch=='e'){
							ptr+=ch;
							state=5;
						}else{
							it--;
							state=0;
							decimalNum++;
							Cout(ptr,"Decimal");
						}
						break;
					}
					case 5:{
						ch=*it++;
						if(isNum(ch)){
							ptr+=ch;
							state=7;
						}else if(ch=='+'||ch=='-'){
							ptr+=ch;
							state=6;
						}else{
							it--;
							state=0;
							error();
						}
						break;
					}
					case 6:{
						ch=*it++;
						if(isNum(ch)){
							ptr+=ch;
							state=7;
						}else{
							it--;
							state=0;
							error();
						}
						break;
					}
					case 7:{
						ch=*it++;
						if(isNum(ch)){
							ptr+=ch;
							state=7;
						}else{
							it--;
							state=0;
							unsignedNum++;
							Cout(ptr,"Unsigned");
						}
						break;
					}
					case 8:{
						ch=*it++;
						if(ch=='='){
							state=0;
							symbolNum++;
							Cout("<=","LE");
						}else if(ch=='>'){
							state=0;
							symbolNum++;
							Cout("<>","NE");
						}else{
							it--;
							state=0;
							symbolNum++;
							Cout("<","LT");
						}
						break;
					}
					case 9:{
						ch=*it++;
						if(ch=='='){
							state=0;
							symbolNum++;
							Cout(">=","GE");
						}else{
							it--;
							state=0;
							symbolNum++;
							Cout(">","GT");
						}
						break;
					}
					case 10:{
						ch=*it++;
						if(ch=='='){
							state=0;
							symbolNum++;
							Cout("==","-");
						}else{
							it--;
							state=0;
							symbolNum++;
							Cout("=","-");
						}
						break;
					}
					case 11:{
						ch=*it++;
						if(ch=='*'){
							isNotes=true;
							state=12;
						}else if(ch=='/'){
							it=s.end();
						}else{
							it--;
							symbolNum++;
							Cout("/","-");
							state=0;
						}
						break;
					}
					case 12:{
						while(it!=s.end()){
							ch=*it++;
							
							if(ch=='*')
							break;
						}
					//	cout<<*it<<endl;
						if(it!=s.end()&&*it=='/'){
							it++;
							isNotes=false;
							state=0;
						}else state=12;
						break;
					}
					case 13:
						error();
						state=0;
						break;
					case 14:{
						ch=*it++;
						if(ch=='"'){
							isQuate=false;
							Cout(ptr,"quate");
							state=0;
						}else{
							ptr+=ch;
							isQuate=true;
						}
						break;
					}
				}
			}
			if(!ptr.empty()){
				str+=ptr;
				str.pop_back();
			}
		}
};
int main(){
	Lexical * test=new Lexical;
	ifstream testFile("test.txt",ios::in);
	string str;
	while(getline(testFile,str,'\n')){
		test->analysis(str);
	}
	test->check();
	cout<<"/************************/"<<endl;
	test->printInfor();
	testFile.close();
	delete test;
	return 0;
}