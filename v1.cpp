#include <iostream>
#include <string>
#include <fstream>

class Encrypter
{
private:
    std::string InFileName;
    char password;

public:
    Encrypter(std::string FileName, char p)
        : InFileName(FileName), password(p) {}

    void encrypt()
    {
        std::string OutFileName = InFileName + ".enc";
        std::ifstream FileInputStream(InFileName);
        std::ofstream FileOutputStream(OutFileName);

        char byte;
        while (FileInputStream.get(byte))
            FileOutputStream.put(byte ^ password);

        FileInputStream.close();
        FileOutputStream.close();
    }
};

class Decrypter
{
private:
    std::string InFileName;
    char password;

public:
    Decrypter(std::string FileName, char p)
        : InFileName(FileName), password(p) {}

    void decrypt()
    {
        std::string OutFileName = InFileName + ".dec";
        std::ifstream FileInputStream(InFileName);
        std::ofstream FileOutputStream(OutFileName);

        char byte;
        while (FileInputStream.get(byte))
            FileOutputStream.put(byte ^ password);
    }
};

int main()
{
    int password;
    std::cout << "Enter Password:" << " ";
    std::cin >> password;

    Encrypter e("Example.txt", password);
    e.encrypt();

    Decrypter d("Example.txt.enc", password);
    d.decrypt();
    return 0;
}