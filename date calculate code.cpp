#include <iostream>
#include <iomanip>
#include <ctime>
#include <windows.h>

using namespace std;

void textcolor(int color_number){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color_number);
}

class Date:public tm{ //time.h, ctime�� �ִ� tm ����ü�� ������ 
//	friend class CalInterface; �����������̽� CalIntaerface class�� friend 

protected:
	unsigned int tdays; //1�� 1�� 1���� 1�Ϸ� �Ͽ� ����� ��¥�� ��ü �� ���� ���� 
	int mfday; // ����� ���� 1���� ��ġ ����(2015�� 3���� ���� ��� 1���� �Ͽ����̹Ƿ� mfday�� 0) �޷��� ���鶧 �ʿ��� 
	int year; // tm_year�� 1900����� �����ϹǷ� 1�Ϻ��� �����ϴ� ������ ���� ���� 

	static const int ldays[12]; //�� ���� ������ ��¥�� �����ϰ� �ִ� ����迭 

	bool leapCheck(int ayear); //�������� �ƴ����� �Ǵ��ϴ� �Լ� 

	void arrByMDY(void); // ������� �������� ������ ������ ��� 
	void arrByTdays(void); // tdays�� �������� ������ ������ ��� 
		void arrRest(void); // �����, tdays�� �������� ������ ������ ���
			void getDayName(void);	// tm_wday�� �������� �����̸��� ���� 
			char day_name[4]; // ���� �̸��� ����Ǵ� ���ڿ� 

public:
	int color; //���ڻ��� �����ϴ� �Լ� 
	
	Date(void); //������ 
	void setDate(int amon, int amday, int ayear); //������� �޾� �����͸� ���� 
	void setDate(int atdays); //tdays�� �޾� �����͸� ���� 
	void showDate(void); // �����͸� ��� 
	const Date operator +(const int a) const; //+�������Լ� 
	const Date operator -(const int a) const; //-������ �Լ� 
	const int operator -(const Date &A) const; //-������ �Լ� 
};
const int Date::ldays[12]={31,28,31,30,31,30,31,31,30,31,30,31}; //�� ���� ������ ��¥�� 

bool Date::leapCheck(int ayear){
	if(ayear%4==0 && ayear%100!=0 || ayear%400==0) return 1;
	return 0;
}
void Date::arrRest(void){
	tm_yday=-1;
	tm_yday+=tm_mday;	
	for(int i=0;i<tm_mon;i++){
		if(leapCheck(year)&&i==1) tm_yday+=ldays[1]+1;
		else tm_yday+=ldays[i];
	}
	tm_year=year-1900;
	tm_wday=tdays%7;
	getDayName();
	mfday=(tdays-tm_mday+1)%7;
}
void Date::arrByMDY(void){
	int i;
	tdays=0;
	
	tdays+=tm_mday;	
	for(i=0;i<tm_mon;i++){
		if(leapCheck(year)&&i==1) tdays+=ldays[1]+1;
		else tdays+=ldays[i];
	}
	for(i=1;i<year;i++){
		if(leapCheck(i)) tdays+=366;
		else tdays+=365;
	}
	arrRest();
}
void Date::arrByTdays(void){
	int i, temp;
	
	tm_mon=0;
	tm_mday=0;
	year=0;
	
	temp=tdays;
	for(i=0;;i++){
		if(i==0) year++;
		else if(leapCheck(i)){
			if(temp<=366) break;
			temp-=366;
			year++;
		} 
		else{
			if(temp<=365) break;
			temp-=365;
			year++;
		} 
	}
	for(i=0;i<12;i++){
		if(leapCheck(year)&&i==1){
			if(temp-(ldays[1]+1)<=0) break;	
			else temp-=(ldays[1]+1);
		}
		else{
			if(temp-ldays[i]<=0) break;	
			else temp-=ldays[i];
		}
		tm_mon++;
	}
	tm_mday=temp;
	arrRest();
}
void Date::getDayName(void){
	switch(tm_wday){
	case 0: memcpy(day_name,"SUN", 4); break;
	case 1: memcpy(day_name,"MON", 4); break;
	case 2: memcpy(day_name,"TUE", 4); break;
	case 3: memcpy(day_name,"WED", 4); break;
	case 4: memcpy(day_name,"THU", 4); break;
	case 5: memcpy(day_name,"FRI", 4); break;
	case 6: memcpy(day_name,"SAT", 4); break;
	}
}
Date::Date(void){
	time_t raw;
	time(&raw);
	struct tm* curtime;
	curtime=localtime(&raw);
	tm_mon=curtime->tm_mon;
	tm_mday=curtime->tm_mday;
	year=curtime->tm_year+1900;	
	arrByMDY();
	color=0x0007;
}
void Date::setDate(int amon, int amday, int ayear){
	tm_mon=amon-1;
	tm_mday=amday;
	year=ayear;
	arrByMDY();
}
void Date::setDate(int atdays){
	tdays=atdays;
	arrByTdays();
}
void Date::showDate(void){
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), color);
	cout<<setw(2)<<setfill('0')<<tm_mon+1<<'/'<<setw(2)<<tm_mday<<'/'<<setw(4)<<year<<'('<<day_name<<')';
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0x0007);
}
const Date Date::operator +(const int a) const{
	Date B;
	B.tdays=tdays+a;
	B.arrByTdays();
	return B;
}
const Date Date::operator -(const int a) const{
	Date B;
	B.tdays=tdays-a;
	B.arrByTdays();
	return B;
}
const int Date::operator -(const Date &A) const{
	int B;
	B=tdays-A.tdays;
	return B;
}
int main(void){
	
	int num;
	
	Date A;
	Date B;
	Date C;
	
	A.color=0x000A;
	B.color=0x000B;
	
	B.setDate(4,10,2015);
	
	cout<<" Date A: ";A.showDate();cout<<" //������ ���ó�¥ �ڵ��Է� "<<endl;
	cout<<" Date B: ";B.showDate();cout<<" //B.setDate(4,10,2015) ���� ��¥ �Է�"<<endl;
	cout<<" B-A= "<<B-A<<" //�����ȳ�¥(B)���� ���ó�¥(A)�� ���� ��ĥ ���Ҵ��� ���"<<endl;
	A=A+20;
	cout<<" A=A+20: ";A.showDate();cout<<" //���ó�¥(A)�� 20�� �� ��¥ ���"<<endl; 
	
	getchar(); 
}
