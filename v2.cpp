#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include <cmath>

class Encrypter
{
private:
    std::string inFileName, outFileName;
    int encryptionPassword;
    std::vector<char> inFileContent;
    std::vector<int> encryptedInFileContent;

public:
    Encrypter(std::string srcFileName, std::string dstFileName, int password)
        : inFileName(srcFileName), outFileName(dstFileName), encryptionPassword(password) {}

private:
    int fillInt(std::vector<char>::iterator rangeStart, std::vector<char>::iterator rangeEnd)
    {
        // Fills an empty (zero) integer with the bytes from the bytes vector
        // which are in the given range. The range can have any length
        // between 1 and 4. The function is designed to avoid having to
        // pad the bytes with zeros (in order to get 4 bytes to form an integer).
        int result = 0;

        for (
            std::vector<char>::const_iterator current = rangeStart;
            current != rangeEnd;
            current++)
        {
            int value = 0 | (unsigned char)*current;
            value = value << (rangeEnd - current - 1) * 8;
            result = result | value;
        }

        return result;
    }

    std::array<char, 4> splitInt(int value)
    {
        // Splits the given integer into 4 bytes. The bytes
        // are returned in a character array where the first
        // element represents the LSB and the last element
        // represents the MSB.
        std::array<char, 4> bytes;

        for (int i = 0; i < 4; i++)
            bytes[i] = (unsigned char)(value >> (i * 8)) & 255;

        return bytes;
    }

    int readInFileContent()
    {
        // Reads the input file's content and returns the
        // size of the created vector.
        std::ifstream fileInputStream(inFileName);
        if (!fileInputStream.is_open())
            return 0;

        char byte;
        while (fileInputStream.get(byte))
            inFileContent.push_back(byte);

        fileInputStream.close();
        return inFileContent.size();
    }

    int min(int a, int b)
    {
        return a < b ? a : b;
    }

    void encryptInFileContent()
    {
        // Takes bytes from the input vector and creates an
        // integer which is encrypted using the given password and
        // put in the output vector.
        std::vector<char>::iterator start = inFileContent.begin();
        std::vector<char>::iterator end = inFileContent.end();

        while (start != end)
        {
            // Tries to take 4 bytes to convert them into an integer.
            // If there aren't 4 bytes left in the input vector then
            // all of the remaining bytes are taken.
            int rangeLength = min(end - start, 4);

            int value = fillInt(start, start + rangeLength);
            // The created integer is encrypted using the XOR operation
            // with the given password.
            value = value ^ encryptionPassword;

            encryptedInFileContent.push_back(value);

            start += rangeLength;
        }
    }

    void exportEncryptedInFileContent()
    {
        // Goes through the output vector, splits the created integers
        // and puts their bytes into the output file.
        std::ofstream outputFileStream(outFileName);

        for (
            std::vector<int>::const_iterator current = encryptedInFileContent.begin();
            current != encryptedInFileContent.end();
            current++)
        {
            // Splits the integer into bytes.
            std::array<char, 4> bytes = splitInt(*current);

            // Because the bytes are returned in an array where
            // the first element represents the LSB and the last element
            // represent the MSB the loop goes backwards through the array.
            for (
                std::array<char, 4>::reverse_iterator currentByte = bytes.rbegin();
                currentByte != bytes.rend();
                currentByte++)
                if (*currentByte)
                    outputFileStream.put(*currentByte);
        }
    }

public:
    static bool isFileEncrypted(std::string fileName)
    {
        return fileName.find(".enc") != std::string::npos;
    }

    static std::string getOutFileName(std::string inFileName)
    {
        // Removes the path from the file name and adds the
        // output path.
        int lastForwardSlashIndex = inFileName.find_last_of("/");
        if (lastForwardSlashIndex == std::string::npos)
            return "Output/" + inFileName;

        return "Output/" + inFileName.substr(lastForwardSlashIndex + 1);
    }

    static std::string removeEncryptionExtension(std::string encryptedFileName)
    {
        int extensionIndex = encryptedFileName.find(".enc");
        if (extensionIndex == std::string::npos)
            return encryptedFileName;
        return encryptedFileName.substr(0, extensionIndex);
    }

    static std::string addEncryptionExtension(std::string fileName)
    {
        return fileName + ".enc";
    }

    void encrypt()
    {
        int size = readInFileContent();
        if (!size)
        {
            std::cout << "Ulazni fajl ne postoji ili je prazan!" << std::endl;
            return;
        }

        encryptInFileContent();
        exportEncryptedInFileContent();
    }
};

int main(int argc, char *argv[])
{
    // Example usage:
    // g++ v2.cpp && ./a.out Input/Example.txt
    if (argc != 2)
    {
        std::cout << "Incorrect usage! You must provide a input file." << std::endl;
        return EXIT_FAILURE;
    }

    if (Encrypter::isFileEncrypted(argv[1]))
    {
        int password;
        std::cout << "The given file is encrypted. Please enter the password." << std::endl;
        std::cout << "Password:" << " ";
        std::cin >> password;

        std::string inFileName = argv[1];
        std::string outFileName = Encrypter::getOutFileName(Encrypter::removeEncryptionExtension(inFileName));

        Encrypter e(inFileName, outFileName, password);
        e.encrypt();

        std::cout << "File decrypted successfully! Output location:" << " " << outFileName << std::endl;

        return EXIT_SUCCESS;
    }

    int password;
    std::cout << "You need to set a password." << std::endl;
    std::cout << "Password:" << " ";
    std::cin >> password;

    std::string inFileName = argv[1];
    std::string outFileName = Encrypter::addEncryptionExtension(Encrypter::getOutFileName(inFileName));
    Encrypter e(inFileName, outFileName, password);
    e.encrypt();

    std::cout << "File encrypted successfully! Output location:" << " " << outFileName << std::endl;
}