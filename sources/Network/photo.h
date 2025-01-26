#pragma once

#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <windows.h>


class Photo {
public:
    Photo(const std::string& photoPath = "")
        : m_photoPath(photoPath), m_size(0) {
        if (photoPath != "") {
            updateSize();
            saveToFile();
        }
    }

    const std::string& getPhotoPath() const { return m_photoPath; }
    void setPhotoPath(std::string& photoPath) { m_photoPath = photoPath; updateSize(); }
    const std::size_t getSize() const { return m_size; }
    std::string serialize();
    static Photo deserialize(const std::string& data);
    void saveToFile() const;

private:
    static std::string wideStringToString(const WCHAR* wideStr);
    void updateSize();

private:
    std::string m_photoPath;
    std::size_t m_size;
};