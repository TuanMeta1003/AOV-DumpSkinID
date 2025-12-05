#pragma once

#include <unordered_set>
#include <unordered_map>

void saveToFile(const std::string& content) {
    static bool wroteHeader = false;
    static std::string packageName(GetPackageName());
    static std::string filePath =
        "/storage/emulated/0/Android/data/" + packageName + "/files/HeroSkinData.txt";

    std::ofstream file(filePath, std::ios::app);
    if (!file.is_open()) return;

    if (!wroteHeader) {
        std::ifstream check(filePath);
        if (check.peek() == std::ifstream::traits_type::eof()) {
            file <<
                "/*\n"
                "\tCredit\n"
                "\t• Telegram: @TuanMeta\n"
                "\t• Telegram Channel: https://t.me/ZRTChannel\n"
                "\t• Telegram Group: https://t.me/ZRTGroup\n"
                "*/\n";
        }
        wroteHeader = true;
    }

    file << content;
    file.flush();
}

static String *(*GetHeroName)(uint32_t heroId); //Hero Name
static String *(*GetSkinName)(uint32_t skinUniId); //Hero Skin

std::unordered_map<std::string, uint32_t> heroNameToId; 
std::unordered_map<uint32_t, std::vector<std::pair<uint32_t, std::string>>> heroSkins;
std::unordered_set<uint32_t> writtenHeroIds;
std::unordered_set<uint32_t> dumpedConfigIds;

void saveHeroData(uintptr_t instance) {
    uint32_t configId = *(uint32_t *)(instance + Field("AovTdr.dll", "ResData", "ResHeroSkin", "dwID"));
    uint32_t heroId   = *(uint32_t *)(instance + Field("AovTdr.dll", "ResData", "ResHeroSkin", "dwHeroID"));

    // Nếu configId đã dump rồi -> bỏ qua
    if (!dumpedConfigIds.insert(configId).second) {
        return;
    }

    std::string heroName = GetHeroName(heroId)->CString();
    if (heroName.empty() || heroName.find("[ex]") != std::string::npos) return;

    auto it = heroNameToId.find(heroName);
    if (it != heroNameToId.end() && it->second != heroId) return;

    heroNameToId[heroName] = heroId;

    std::string skinName = GetSkinName(configId)->CString();
    if (skinName.empty()) return;

    if (skinName.find("[ex]") != std::string::npos) {
        skinName = "Skin ID Trống";
    }

    heroSkins[heroId].emplace_back(configId, skinName);

    std::stringstream data;

    // Ghi tiêu đề hero 1 lần
    if (writtenHeroIds.insert(heroId).second) {
        data << "\n" << heroName << " [" << heroId << "]:\n";
    }

    int index = heroSkins[heroId].size(); 
    data << index << ". " << skinName << " : " << configId << "\n";

    saveToFile(data.str());
}

int (*_TransferData)(uintptr_t instance, void *trans);
int TransferData(uintptr_t instance, void *trans) {
	auto result = _TransferData(instance, trans);
	saveHeroData(instance);
	return result;
}
