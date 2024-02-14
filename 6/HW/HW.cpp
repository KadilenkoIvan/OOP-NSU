#include <iostream>
#include <string>

void SendSms(const std::string& number, const std::string& message) {
	std::cout << "Send " << message << " to number " << number << '\n';
}
void SendEmail(const std::string& email, const std::string& message) {
	std::cout << "Send " << message << " e - mail " << email << '\n';
}
class INotifier
{
public:
	virtual void Notify(const std::string& message) = 0;
};
class SmsNotifier : public INotifier
{
private:
	std::string number;

public:
	SmsNotifier(const std::string& new_number)
	{
		number = new_number;
	}
	void Notify(const std::string& message) override
	{
		SendSms(number, message);
	}
};
class EmailNotifier : public INotifier
{
private:
	std::string email;

public:
	EmailNotifier(const std::string& new_email)
	{
		email = new_email;
	}
	void Notify(const std::string& message) override
	{
		SendEmail(email, message);
	}
};

void Notify(INotifier& obj, const std::string& message)
{
	obj.Notify(message);
}

int main() {
	SmsNotifier sms{"+7-923-228-38-72"};
	EmailNotifier email{"m.Filippov@g.nsu.ru"};
	Notify(sms, "I know lessons in C++");
	Notify(email, "And want the lesson one");
	return 0;
}
