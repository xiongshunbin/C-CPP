#include<iostream>

int main() {
	using namespace std;
	const int inch_per_foot = 12;
	const double inch_to_meter = 0.0254;
	const double kilogram_to_pound = 2.2;

	int foot;
	int inch;
	cout << "Please enter your height\n";
	cout << "foot:______\b\b\b\b";
	cin >> foot;
	cout << "inch:______\b\b\b\b";
	cin >> inch;
	double height = (foot * inch_per_foot + inch) * inch_to_meter;
	double weight_pound,weight;
	cout << "Enter your weight:______\b\b\b\b";
	cin >> weight_pound;
	weight = weight_pound / kilogram_to_pound;
	double bmi =weight  /(height*height) ;
	cout << "Your BMI is " << bmi << ".\n";

	return 0;
}
