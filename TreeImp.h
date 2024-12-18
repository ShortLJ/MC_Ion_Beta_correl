//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Nov 20 15:42:56 2024 by ROOT version 6.30/09
// from TTree treeImp/treeImp
// found on file: treeSort.root
//////////////////////////////////////////////////////////

#ifndef TreeImp_h
#define TreeImp_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class TreeImp {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Long64_t        ievt;
   UInt_t          ispecies;
   Double_t        Timp;
   Double_t        Ximp;
   Double_t        Yimp;

   // List of branches
   TBranch        *b_ievt;   //!
   TBranch        *b_ispecies;   //!
   TBranch        *b_Timp;   //!
   TBranch        *b_Ximp;   //!
   TBranch        *b_Yimp;   //!

   TreeImp(TTree *tree=0);
   virtual ~TreeImp();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef TreeImp_cxx
TreeImp::TreeImp(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("treeSort.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("treeSort.root");
      }
      f->GetObject("treeImp",tree);

   }
   Init(tree);
}

TreeImp::~TreeImp()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TreeImp::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TreeImp::LoadTree(Long64_t entry)
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

void TreeImp::Init(TTree *tree)
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

   fChain->SetBranchAddress("ievt", &ievt, &b_ievt);
   fChain->SetBranchAddress("ispecies", &ispecies, &b_ispecies);
   fChain->SetBranchAddress("Timp", &Timp, &b_Timp);
   fChain->SetBranchAddress("Ximp", &Ximp, &b_Ximp);
   fChain->SetBranchAddress("Yimp", &Yimp, &b_Yimp);
   Notify();
}

Bool_t TreeImp::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TreeImp::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TreeImp::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef TreeImp_cxx
