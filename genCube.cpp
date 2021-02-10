#include <iostream>
#include <string>
using namespace std;

string v1, v2, v3, v4, v5, v6, v7, v8;

void printTriangle1(string a, string b, string c) {
	cout << "\t" << a << "0.0, 1.0," << endl 
		 << "\t" << b << "1.0, 1.0," << endl 
		 << "\t" << c << "1.0, 0.0," << endl;
}

void printTriangle2(string a, string b, string c) {
	cout << "\t" << a << "1.0, 0.0," << endl
		 << "\t" << b << "0.0, 0.0," << endl 
		 << "\t" << c << "0.0, 1.0," << endl;
}

void printSquare(string a, string b, string c, string d) {
	printTriangle1(a, b, c);
	printTriangle2(c, d, a);
}

int main() {
	//front 4
	v1 = "-0.5, 0.5, 0.5,";
	v2 = " 0.5, 0.5, 0.5,";
	v3 = " 0.5,-0.5, 0.5,";
	v4 = "-0.5,-0.5, 0.5,";

	//back 4
	v5 = "-0.5, 0.5,-0.5,";
	v6 = " 0.5, 0.5,-0.5,";
	v7 = " 0.5,-0.5,-0.5,";
	v8 = "-0.5,-0.5,-0.5,";

	cout << "float vertices[] = {" << endl;

	printSquare(v1, v2, v3, v4); //front
	printSquare(v6, v5, v8, v7); //back
	printSquare(v5, v1, v4, v8); //left
	printSquare(v2, v6, v7, v3); //right
	printSquare(v5, v6, v2, v1); //up
	printSquare(v7, v8, v4, v3); //down
	
	cout << "};" << endl;

	return 0;
}
