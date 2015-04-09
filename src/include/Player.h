#pragma once
#include "Character.h"
class Player : public Character
{
public:
	void Down_K_w();
	void Down_K_a();
	void Down_K_s();
	void Down_K_d();
	void Up_K_w();
	void Up_K_a();
	void Up_K_s();
	void Up_K_d();

	Player();
	~Player();
protected:

};

