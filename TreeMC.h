//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Oct 10 14:45:17 2024 by ROOT version 6.30/07
// from TTree treeMC/treeMC
// found on file: treeMC.root
//////////////////////////////////////////////////////////

#ifndef TreeMC_h
#define TreeMC_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class TreeMC {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        Timp;
   Double_t        Tdec;
   Double_t        dT_Tdec_Timp;

   // List of branches
   TBranch        *b_Timp;   //!
   TBranch        *b_Tdec;   //!
   TBranch        *b_dT_Tdec_Timp;   //!

   TreeMC(TTree *tree=0);
   virtual ~TreeMC();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef TreeMC_cxx
TreeMC::TreeMC(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("treeMC.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("treeMC.root");
      }
      f->GetObject("treeMC",tree);

   }
   Init(tree);
}

TreeMC::~TreeMC()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TreeMC::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TreeMC::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void TreeMC::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Timp", &Timp, &b_Timp);
   fChain->SetBranchAddress("Tdec", &Tdec, &b_Tdec);
   fChain->SetBranchAddress("dT_Tdec_Timp", &dT_Tdec_Timp, &b_dT_Tdec_Timp);
   Notify();
}

Bool_t TreeMC::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TreeMC::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TreeMC::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef TreeMC_cxx
