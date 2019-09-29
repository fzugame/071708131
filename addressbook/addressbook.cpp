#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <fstream>  
using namespace std;
const int pnum = 34;
const char FILENAME[32] = "a.txt";
/*这个文件内是城市名称文件，评测时请您帮我将文件路径补充完整（这取决于您将下载的文件存放在哪）*/
wstring pc[400];
class Indexes
{
public:
	int province[34] = { 0 };
	int wnum;
	Indexes();

};

class Person1
{
public:
	wstring  province;
	wstring city, area, town, leftaddress;
	Person1();
	void getaddress_simple(wstring&, char*, const Indexes&);
};
Person1::Person1() : province(L"\"\""), area(L"\"\""), city(L"\"\""), town(L"\"\""), leftaddress(L"\"\"") {};

class Person2 :public Person1
{
public:
	wstring villiage, detail, hnum;
	Person2() :Person1(), villiage(L""), detail(L""), hnum(L"") {};
	void getperson(Person1&);
	void getaddress_left();
};

void Person2::getperson(Person1& sone)
{
	province = sone.province;
	city = sone.city;
	area = sone.area;
	town = sone.town;
	leftaddress = sone.leftaddress;
}




wstring change_to_w(string& str)
{
	wstring_convert<std::codecvt_utf8<wchar_t>> cov;
	wstring wstr = cov.from_bytes(str);
	return wstr;
}

string change_to_s(wstring& result)
{
	wstring_convert<std::codecvt_utf8<wchar_t>> cov;
	string result2 = cov.to_bytes(result);
	return result2;
}

void print_to_file(string& result, char* argc)
{
	ofstream fout;
	fout.open(argc, ios::out | ios::app);
	fout << result;
	fout.close();
	fout.clear();
}


void getname(wstring& wstr, char* argc)
{
	wstring_convert<std::codecvt_utf8<wchar_t>> cov;
	int len = (int)wstr.size();
	wstring result = L"{\"姓名\": \"";
	for (int i = 2; i < len; i++)
	{
		if (wstr[i] == L',')
		{
			break;
		}
		if (wstr[i] != L'!')
			result = result + wstr[i];
	}
	result = result + L"\",";
	string result2;
	result2 = cov.to_bytes(result);
	ofstream fout;
	fout.open(argc, ios::out | ios::app);

	fout << result2;
	fout.close();
	fout.clear();
}
void getphone(wstring& wstr, char* argv)
{
	int len = (int)wstr.size();

	wstring result1 = L"\"手机\":\"";
	wstring result = L"";
	int k, mark;
	for (int i = 2; i < len; i++)
	{

		if (wstr[i] >= L'0' && wstr[i] <= L'9')
		{
			k = i;
			result = wstr[i];
			//wstr[i] = L'!';
			int j;
			for (j = i + 1; j < len; j++)
			{
				if (wstr[j] >= L'0' && wstr[j] <= L'9')
				{
					result = result + wstr[j];
					//wstr[j] = L'!';
				}
				else break;

			}
			if (result.size() == 11)
			{
				for (mark = j - 1; mark >= k; mark--)
				{
					wstr[mark] = L'!';
				}
				break;
			}
			else
			{
				result = L"";
				i = j--;
			}
		}
	}

	result = result + L"\",";
	result = result1 + result;
	string result2 = change_to_s(result);
	print_to_file(result2, argv);
}


void Person1::getaddress_simple(wstring& wstr, char* argv, const Indexes& sone)
{
	int i = 2, mark = 0;
	while (wstr[i] != L',')i++;
	int len = (int)wstr.size();
	wstring result = L"\"地址\":[\"";

	//开始处理省级单位
	i++;
	int flag = 0;
	province = L"";
	while (flag < 2)
	{
		if (wstr[i] != L'!')
		{
			province = province + wstr[i];
			flag++;
		}
		i++;
	}
	if (province == L"北京" || province == L"重庆" || province == L"上海" || province == L"天津")
	{
		//cout << "hello" << endl;
		city = province + L"市";
		while (wstr[i] == L'!' || wstr[i] == L'市')
		{
			i++;
		}

	}
	else
	{
		wstring lin = L"";
		int j;
		for (j = 0; j < pnum; j++)
		{
			lin = lin + pc[sone.province[j]][0] + pc[sone.province[j]][1];
			if (province == lin)
			{
				//cout << "yes there is"<<endl;
				province = pc[sone.province[j]];
				break;
			}
			lin = L"";
		}
		mark = j;
		int len = (int)province.size();
		j = 2;
		while (j < len || wstr[i] == L'!')
		{
			if ((wstr[i] != L'!') && (wstr[i] != province[j]))break;
			else if ((wstr[i] != L'!') && (wstr[i] == province[j])) { j++; i++; }
			else i++;
		}

		//开始处理市级单位
		city = L"";
		city = city + wstr[i];
		i++;
		city = city + wstr[i];
		flag = 0;
		for (j = sone.province[mark] + 1; j < sone.province[mark + 1]; j++)
		{
			lin = L"";
			lin = lin + pc[j][0];
			lin = lin + pc[j][1];
			if (lin == city)
			{
				flag = 1;
				len = 0;
				city = L"";
				while (pc[j][len] != L':')
				{
					city = city + pc[j][len];
					len++;
				}
				break;
			}
		}


		//将数组下标i 移动到区级单位开始的位置
		if (flag == 1)
		{
			j = 2; i++;
			len = (int)city.size();
			while (j < len || wstr[i] == L'!')
			{
				if ((wstr[i] != L'!') && (wstr[i] != city[j]))break;
				else if ((wstr[i] != L'!') && (wstr[i] == city[j])) { i++; j++; }
				else i++;
			}
		}
		else
		{
			city = L"";
			i--;
		}





	}
	//处理区；
	area = L"";
	len = (int)wstr.size();
	flag = 0;
	mark = i;
	while (i < len)
	{
		if (wstr[i] != L'!')
			area = area + wstr[i];
		if (wstr[i] == L'区' || wstr[i] == L'旗' || wstr[i] == L'县' || wstr[i] == L'市')
		{
			flag = 1;
			i++;
			break;
		}
		i++;

	}
	if (flag == 0)
	{
		i = mark;
		area = L"";
	}

	//处理镇，乡，盟，街道,开发区
	mark = i;
	town = L"";
	flag = 0;
	while (i < len)
	{
		if (wstr[i] != L'!')
			town = town + wstr[i];
		if (wstr[i] == L'镇' || wstr[i] == L'乡')
		{
			i++;
			flag = 1;
			break;
		}
		else if (wstr[i] == L'街' && i + 1 < len && wstr[i + 1] == L'道')
		{
			flag = 1;
			i++;
			town = town + wstr[i];
			i++;
			break;
		}
		else if (wstr[i] == L'开' && i < len - 2 && wstr[i + 1] == L'发' && wstr[i + 2] == L'区')
		{
			flag;
			town = town + L"发区";
			i = i + 3;
			break;
		}
		i++;
	}
	if (flag == 0)
	{
		town = L"";
		i = mark;
	}
	//处理剩余地址
	leftaddress = L"";
	while (i < len)
	{
		if (wstr[i] != L'!' && wstr[i] != L'.')
			leftaddress = leftaddress + wstr[i];
		i++;
	}
	result = result + province + L"\",\"" + city + L"\",\"" + area + L"\",\"" + town + L"\",";
	string result2 = change_to_s(result);
	print_to_file(result2, argv);

}

Indexes::Indexes()
{
	wnum = 0;
	int p[34] = { 0,3,17,29,47,57,69,87,91,94,
		106,118,131,146,156,170,189,204,219,241,
		256,259,267,289,299,316,324,335,350,359,
		365,381,386,391 };
	for (int i = 0; i < 34; i++)
	{
		province[i] = p[i];
	}
	ifstream fin(FILENAME);
	string str;
	while (!fin.eof())
	{
		//cout << "1" << endl;
		getline(fin, str);
		pc[wnum++] = change_to_w(str);
	}
	fin.close();
	fin.clear();

}


void Person2::getaddress_left()
{
	int lenth;
	lenth = (int)leftaddress.size();
	int i = 0;
	int flag = 0;
	while (i < lenth)
	{
		if (leftaddress[i] >= L'0' && leftaddress[i] <= L'9')
		{
			flag = 1;
			break;
		}
		villiage = villiage + leftaddress[i];
		i++;
	}
	if (flag == 0)
	{
		villiage = L"";
		hnum = L"";
		i = 0;
	}
	else
	{
		while (i < lenth)
		{
			hnum = hnum + leftaddress[i];
			if (leftaddress[i] >= L'0' && leftaddress[i] <= L'9' && i + 1 < lenth && leftaddress[i + 1] == L'号')
			{
				hnum = hnum + L'号';
				i += 2;
				break;
			}
			i++;
		}


	}
	while (i < lenth)
	{
		detail = detail + leftaddress[i];
		i++;
	}

}

int main(int argc, char* argv[])
{
	//char argv[3][15] = { " ","fin.txt","b.txt" };
	//声明
	wstring change_to_w(string&);
	void getname(wstring&, char*);
	void getphone(wstring&, char*);

	//实现
	string str;
	Indexes sone;
	ifstream fin(argv[1]);
	codecvt_utf8<wchar_t>* FPPPS;
	auto LocUtf8 = locale(locale(""), FPPPS = new codecvt_utf8<wchar_t>);
	fin.imbue(LocUtf8);
	ofstream fout(argv[2]);
	fout << "[" << endl;
	fout.close();
	fout.clear();
	int number = 0;
	wstring wstr = L"", lin = L"";
	while (!fin.eof())
	{
		if (number != 0)
		{
			lin = L",\n";
			str = change_to_s(lin);
			print_to_file(str, argv[2]);
		}
		getline(fin, str);
		wstr = change_to_w(str);
		getname(wstr, argv[2]);
		getphone(wstr, argv[2]);
		Person1 person;
		person.getaddress_simple(wstr, argv[2], sone);
		if (wstr[0] != L'1' && wstr[0] != L'2' && wstr[0] != L'3')
		{
			wstr[0] = wstr[1];
		}
		if (wstr[0] == L'1')
		{
			lin = L"";
			lin = L'\"' + person.leftaddress + L"\"]}";
			string lin2 = change_to_s(lin);
			print_to_file(lin2, argv[2]);
		}
		else
		{
			lin = L"";
			Person2 view;
			view.getperson(person);
			view.getaddress_left();
			lin = L"\"" + view.villiage + L"\",\"" + view.hnum + L"\",\"" + view.detail + L"\"]}";
			string lin2 = change_to_s(lin);
			print_to_file(lin2, argv[2]);

		}
		number++;
	}

	lin = L"\n]";
	str = change_to_s(lin);
	print_to_file(str, argv[2]);
	return 0;

}