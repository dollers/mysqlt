

class Pause
{
public:
	Pause(float time);
	~Pause();
	void setTime(float time);

private:
	float time;

public:
	bool pause();
	bool pause(float time);
};