#include <iostream>
#include <iomanip>
#include <ctime>
#include <windows.h>

using namespace std;

void textcolor(int color_number){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color_number);
}

class Date:public tm{ //time.h, ctime에 있는 tm 구조체를 가져옴 
//	friend class CalInterface; 최종인터페이스 CalIntaerface class에 friend 

protected:
	unsigned int tdays; //1년 1월 1일을 1일로 하여 저장된 날짜의 전체 일 수를 저장 
	int mfday; // 저장된 달의 1일의 위치 저장(2015년 3월을 예를 들면 1일이 일요일이므로 mfday는 0) 달력을 만들때 필요함 
	int year; // tm_year는 1900년부터 시작하므로 1일부터 시작하는 변수를 따로 저장 

	static const int ldays[12]; //각 달의 마지막 날짜를 저장하고 있는 상수배열 

	bool leapCheck(int ayear); //윤년인지 아닌지를 판단하는 함수 

	void arrByMDY(void); // 년원일을 기준으로 나머지 정보를 계산 
	void arrByTdays(void); // tdays를 기준으로 나머지 정보를 계산 
		void arrRest(void); // 년월일, tdays를 기준으로 나머지 정보를 계산
			void getDayName(void);	// tm_wday를 기준으로 요일이름을 만듦 
			char day_name[4]; // 요일 이름이 저장되는 문자열 

public:
	int color; //글자색을 저장하는 함수 
	
	Date(void); //생성자 
	void setDate(int amon, int amday, int ayear); //년월일을 받아 데이터를 만듦 
	void setDate(int atdays); //tdays를 받아 데이터를 만듦 
	void showDate(void); // 데이터를 출력 
	const Date operator +(const int a) const; //+연사자함수 
	const Date operator -(const int a) const; //-연산하 함수 
	const int operator -(const Date &A) const; //-연산자 함수 
};
const int Date::ldays[12]={31,28,31,30,31,30,31,31,30,31,30,31}; //각 달의 마지막 날짜들 

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
	
	cout<<" Date A: ";A.showDate();cout<<" //생성시 오늘날짜 자동입력 "<<endl;
	cout<<" Date B: ";B.showDate();cout<<" //B.setDate(4,10,2015) 으로 날짜 입력"<<endl;
	cout<<" B-A= "<<B-A<<" //지정된날짜(B)에서 오늘날짜(A)를 빼서 며칠 남았는지 출력"<<endl;
	A=A+20;
	cout<<" A=A+20: ";A.showDate();cout<<" //오늘날짜(A)에 20일 후 날짜 계산"<<endl; 
	
	getchar(); 
}
