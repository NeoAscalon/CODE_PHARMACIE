#pragma once
#include <wiringPi.h>
#include <string>
#include <iostream>


class SasDePharmacie
{
public:
	SasDePharmacie();
	~SasDePharmacie();
	void PrisePhoto();
	void LectureBadge();
	void SwitchPorte();
	void CapteurPIR();
	static string print_hex(const uint8_t*, const size_t);
private:
	string  UID;

};

