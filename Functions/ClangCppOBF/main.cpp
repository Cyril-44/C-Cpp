#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Frontend/CompilerInstance.h>

using namespace clang;
using namespace clang::tooling;

class V : public RecursiveASTVisitor<V> {
public:
    Rewriter &R;
    std::map<const VarDecl*, std::string> mp;
    int id = 0;

    V(Rewriter &R) : R(R) {}

    std::string get(const VarDecl *D) {
        if (!mp.count(D))
            mp[D] = D->getName().str() + "_" + std::to_string(id++);
        return mp[D];
    }

    bool VisitVarDecl(VarDecl *D) {
        if (D->getIdentifier())
            R.ReplaceText(D->getLocation(), D->getName().size(), get(D));
        return true;
    }

    bool VisitDeclRefExpr(DeclRefExpr *E) {
        if (auto *D = dyn_cast<VarDecl>(E->getDecl()))
            if (D->getIdentifier())
                R.ReplaceText(E->getLocation(), D->getName().size(), get(D));
        return true;
    }
};

class A : public ASTFrontendAction {
public:
    Rewriter R;

    std::unique_ptr<ASTConsumer> CreateASTConsumer(
        CompilerInstance &CI, StringRef) override {

        R.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());

        class C : public ASTConsumer {
        public:
            V visitor;

            C(Rewriter &R) : visitor(R) {}

            void HandleTranslationUnit(ASTContext &Ctx) override {
                visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
                visitor.R.getEditBuffer(
                    Ctx.getSourceManager().getMainFileID()
                ).write(llvm::outs());
            }
        };

        return std::make_unique<C>(R);
    }
};

int main(int argc, const char **argv) {
    static llvm::cl::OptionCategory Cat("tool");
    auto& P = CommonOptionsParser::create(argc, argv, Cat).get();
    return ClangTool(P.getCompilations(), P.getSourcePathList())
        .run(newFrontendActionFactory<A>().get());
}