// 重建自 ArbiterPlus —— c-s/common/config.h
#ifndef AP_CONFIG_H
#define AP_CONFIG_H

#include <string>
#include <map>

// [节] field = value 的 INI 风格配置; '#' 为注释。
// 注: 实际存储为 "节 field" 层级键 —— 见 getValue 之 field 形如 "network ServerPort"
class Config {
public:
    Config() {}
    Config(const std::string& filename) { load(filename); }

    void load(const std::string& filename);
    bool read(const std::string& line, std::string& field, std::string& value);

    std::string getValue(const std::string& field, const std::string& def);
    int         getNumericValue(const std::string& field, int def);   // 还原: atoi(getValue)

private:
    std::map<std::string, std::string> m_values;
};

// [network] 节端口集合 —— NetworkConfig 构造于 inetwork.h
#endif
