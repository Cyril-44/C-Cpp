#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Frontend/CompilerInstance.h>

using namespace clang;
using namespace clang::tooling;

#include <string>
#include <random>
#include <cstddef>


llvm::cl::OptionCategory OBFCategory("OBF Options");

llvm::cl::opt<bool> compress(
    "compress",
    llvm::cl::desc("Use compressed variable names (a, b, c, ...)"),
    llvm::cl::init(false),
    llvm::cl::cat(OBFCategory)
);

llvm::cl::opt<size_t> minLen(
    "min-len",
    llvm::cl::desc("Minimum random variable name length"),
    llvm::cl::init(12),
    llvm::cl::cat(OBFCategory)
);

llvm::cl::opt<size_t> maxLen(
    "max-len",
    llvm::cl::desc("Maximum random variable name length"),
    llvm::cl::init(20),
    llvm::cl::cat(OBFCategory)
);

class OBFStringGenerator {
private:
    size_t counter = 0;
    std::mt19937_64 rng;

    static constexpr char firstChars[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"; // 53 chars
    static constexpr char otherChars[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789"; // 63 chars

    static constexpr size_t FIRST = 53;
    static constexpr size_t OTHER = 63;

    // 高效的高进制编码器
    std::string nextCompressed() {
        size_t x = counter++;
        std::string s;

        // 首字符：53 进制
        s.push_back(firstChars[x % FIRST]);
        x /= FIRST;

        // 后续字符：63 进制
        while (x > 0) {
            s.push_back(otherChars[x % OTHER]);
            x /= OTHER;
        }

        return s;
    }

    // 生成随机模式下的名字
    std::string nextRandom() {
        std::uniform_int_distribution<size_t> lenDist(minLen, maxLen);
        size_t len = lenDist(rng);

        std::string s;
        s.reserve(len);

        // 首字符
        {
            std::uniform_int_distribution<int> d(0, sizeof(firstChars) - 2);
            s.push_back(firstChars[d(rng)]);
        }

        // 后续字符
        {
            std::uniform_int_distribution<int> d(0, sizeof(otherChars) - 2);
            for (size_t i = 1; i < len; ++i)
                s.push_back(otherChars[d(rng)]);
        }

        return s;
    }

public:
    OBFStringGenerator(uint64_t seed = std::random_device{}()) : rng(seed) {}

    // 唯一接口：获取一个可作为 C++ 变量名的字符串
    std::string next() {
        if (compress)
            return nextCompressed();
        else
            return nextRandom();
    }
} gen;

struct V : public RecursiveASTVisitor<V> {
    Rewriter &R;
    ASTContext &Context;          // 新增：用于在依赖上下文中查找成员声明
    std::map<const Decl*, std::string> mp;
    std::set<SourceLocation> processed;   // 新增：防止同一源码位置被多次替换（模板函数声明处经典问题）
    int id = 0;

    V(Rewriter &R, ASTContext& ctx) : R(R), Context(ctx) {}

    // ==================== 关键修复：统一所有 Decl* 为同一个 key ====================
    const Decl* getKey(const NamedDecl *D_) {
        if (!D_) return nullptr;
        auto D = D_->getCanonicalDecl();

        // 1. 函数模板的声明处（FunctionTemplateDecl）
        if (const auto *FT = dyn_cast<FunctionTemplateDecl>(D))
            return FT;

        // 2. 普通函数 / 成员函数模板的声明处
        if (const auto *FD = dyn_cast<FunctionDecl>(D)) {
            // 调用处（x.read）拿到的往往是特化后的 FunctionDecl
            if (const auto *Primary = FD->getPrimaryTemplate())
                return getKey(Primary);                    // 递归回到主模板

            if (const auto *Pattern = FD->getTemplateInstantiationPattern())
                return getKey(Pattern);                    // 递归回到实例化源

            if (const auto *FT = FD->getDescribedFunctionTemplate())
                return FT->getCanonicalDecl();

            if (const auto *MD = dyn_cast<CXXMethodDecl>(FD)) {
                if (const auto *FT = MD->getDescribedFunctionTemplate())
                    return FT->getCanonicalDecl();
            }
        }

        // 3. 类模板（如果 Demical 是模板类）
        if (const auto *TD = dyn_cast<TagDecl>(D)) {
            if (const auto *CT = TD->getDescribedTemplate())
                return CT->getCanonicalDecl();
        }

        // 4. UsingShadowDecl 等间接声明
        if (const auto *USD = dyn_cast<UsingShadowDecl>(D))
            if (const auto *Target = USD->getTargetDecl())
                return getKey(Target);

        return D;
    }

    std::string get(const NamedDecl *D) {
        if (!D) return "";
        const Decl *Key = getKey(D);
        if (!mp.count(Key)) {
            // std::string newName = D->getName().str() + "_" + std::to_string(id++);
            std::string newName = gen.next();
            mp[Key] = newName;
            llvm::errs() << "[DEBUG] New mapping: " << D->getName().str()
                         << "  -->  " << newName << "  (Key: " << Key << ")\n";
        }
        return mp[Key];
    }

    // 带防重复保护的 replace（彻底解决 read_12_12）
    inline bool replace(NamedDecl *D, SourceLocation loc, const char* visitorName = "Unknown") {
        if (!D || !loc.isValid()) return true;
        if (!R.getSourceMgr().isWrittenInMainFile(D->getLocation())) return true;
        if (!D->getIdentifier()) return true;

        // 同一源码位置只允许替换一次（模板函数声明处会被 VisitNamedDecl 打两次）
        if (processed.count(loc)) return true;
        processed.insert(loc);

        std::string newName = get(D);

        auto &SM = R.getSourceMgr();
        unsigned line = SM.getSpellingLineNumber(loc);
        unsigned col  = SM.getSpellingColumnNumber(loc);

        llvm::errs() << "[REPLACE] " << visitorName 
                     << " @ " << line << ":" << col 
                     << "   '" << D->getName().str() 
                     << "'  -->  '" << newName << "'\n";

        R.ReplaceText(loc, D->getName().size(), newName);
        return true;
    }

    // ==================== 其余 Visitors（无需改动） ====================
    bool VisitNamedDecl(NamedDecl *D) { 
        return replace(D, D->getLocation(), "VisitNamedDecl"); 
    }

    bool VisitDeclRefExpr(DeclRefExpr *E) {
        if (auto *D = dyn_cast<NamedDecl>(E->getDecl()))
            return replace(D, E->getLocation(), "VisitDeclRefExpr");
        return true;
    }

    bool VisitMemberExpr(MemberExpr *E) {          // ← 关键：x.read 走这里
        if (auto *D = dyn_cast<NamedDecl>(E->getMemberDecl()))
            return replace(D, E->getMemberLoc(), "VisitMemberExpr");
        return true;
    }

    bool VisitCXXConstructorDecl(CXXConstructorDecl *Ctor) {
        if (auto *RD = dyn_cast<NamedDecl>(Ctor->getParent()))
            replace(RD, Ctor->getNameInfo().getLoc(), "VisitCXXConstructorDecl (RD)");
        for (auto *Init : Ctor->inits())
            if (Init->isMemberInitializer() && Init->getSourceOrder() >= 0)
                if (auto *D = dyn_cast<NamedDecl>(Init->getMember()))
                    replace(D, Init->getMemberLocation(), "VisitCXXConstructorDecl (Init)");
        return true;
    }

    bool VisitCXXDependentScopeMemberExpr(CXXDependentScopeMemberExpr *E) {
        // To be implemented
        return true;
    }


    // 类型名引用（struct/class/enum）
    bool VisitTagTypeLoc(TagTypeLoc L) {
        if (L.isDefinition()) return true;
        if (auto *D = dyn_cast<NamedDecl>(L.getDecl())) return replace(D, L.getNameLoc(), "VisitTagTypeLoc");
        return true;
    }
    // 模板特化
    bool VisitTemplateSpecializationTypeLoc(TemplateSpecializationTypeLoc L) {
        if (auto *D = dyn_cast<NamedDecl>(L.getTypePtr()->getTemplateName().getAsTemplateDecl()))
            return replace(D, L.getTemplateNameLoc(), "VisitTemplateSpecializationTypeLoc");
        return true;
    }

    // 新增：template<typename T> 中的 T 引用（T var;）
    bool VisitTemplateTypeParmTypeLoc(TemplateTypeParmTypeLoc L) {
        if (auto *D = L.getDecl()) return replace(D, L.getNameLoc(), "VisitTemplateTypeParmTypeLoc");
        return true;
    }

    // 新增：using T = ... / typedef ... 中的 T 引用
    bool VisitTypedefTypeLoc(TypedefTypeLoc L) {
        if (auto *D = L.getTypedefNameDecl()) return replace(D, L.getNameLoc(), "VisitTypedefTypeLoc");
        return true;
    }
};


struct A : public ASTFrontendAction {
    Rewriter R;
    std::unique_ptr<ASTConsumer> CreateASTConsumer(
        CompilerInstance &CI, StringRef) override {
        R.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        struct C : public ASTConsumer {
            V visitor;
            C(Rewriter &R, ASTContext& ctx) : visitor(R, ctx) {}
            void HandleTranslationUnit(ASTContext &Ctx) override {
                visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
                visitor.R.getEditBuffer(
                    Ctx.getSourceManager().getMainFileID()
                ).write(llvm::outs());
            }
        };
        return std::make_unique<C>(R, CI.getASTContext());
    }
};

int main(int argc, const char **argv) {
    llvm::cl::HideUnrelatedOptions(OBFCategory);
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, OBFCategory);
    if (!ExpectedParser) return (llvm::errs() << llvm::toString(ExpectedParser.takeError()) << "\n"), 1;
    CommonOptionsParser &P = *ExpectedParser;
    ClangTool Tool(P.getCompilations(), P.getSourcePathList());
    return Tool.run(newFrontendActionFactory<A>().get());
}
