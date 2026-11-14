#ifndef ROBLOXDOC_H
#define ROBLOXDOC_H

#include "ClipboardVerbs.h"
#include "VerbBinder.h"
#include "Workspace.h"
#include "placeholder.h"
#include "v8datamodel/Unknown005d2700.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <string>
#include <vector>

class CRobloxDoc;

class ReportAbuseVerb : public RBX::Verb
{
public:
	ReportAbuseVerb(RBX::VerbContainer* p_unk0x04, CRobloxDoc* p_unk0x08)
		: RBX::Verb(p_unk0x04, "ReportAbuse"), m_unk0x0c(p_unk0x08)
	{
	}

	virtual bool isEnabled() const;
	virtual void doIt(RBX::IDataState* dataState);

private:
	CRobloxDoc* m_unk0x0c;
};

class ExitCommand : public RBX::Verb
{
public:
	ExitCommand(RBX::VerbContainer* p_unk0x04, CRobloxDoc* p_unk0x08)
		: RBX::Verb(p_unk0x04, "Exit"), m_unk0x0c(p_unk0x08)
	{
	}

	virtual void doIt(RBX::IDataState* dataState);

private:
	CRobloxDoc* m_unk0x0c;
};

class CRobloxDoc : public VerbBinder<CRobloxDoc, CDocument>
{
public:
	boost::shared_ptr<RBX::DataModel> FUN_00403290() const
	{
		if (m_unk0x78) {
			return m_unk0x78->FUN_0046edc0();
		}

		return boost::shared_ptr<RBX::DataModel>();
	}

	DECLARE_DYNCREATE(CRobloxDoc)

protected:
	CRobloxDoc();

public:
	virtual ~CRobloxDoc();

	RBX::VerbContainer* FUN_004558a0();
	void FUN_00456c50(int id, RBX::Verb* verb);
	void FUN_00456d30();

	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual BOOL IsModified();
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	virtual void DeleteContents();
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual BOOL SaveModified();
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
	BOOL FUN_00459950(std::string p_unk0x04);
	void FUN_00459c80();
	void FUN_00459da0();

protected:
	afx_msg void FUN_00453eb0(CCmdUI* pCmdUI);
	afx_msg void FUN_00454430();
	afx_msg void FUN_00454920();
	afx_msg void FUN_004549c0();
	afx_msg void FUN_00454a60();
	afx_msg void FUN_00454e50();
	afx_msg void FUN_00454f50();
	afx_msg void FUN_004550b0();
	afx_msg void FUN_00455900();
	afx_msg void FUN_00455a30(CCmdUI* pCmdUI);
	afx_msg void FUN_00455bb0();
	afx_msg void FUN_00455ce0(CCmdUI* pCmdUI);
	afx_msg void FUN_004561a0();
	afx_msg void FUN_00455ff0(CCmdUI* pCmdUI);
	afx_msg void FUN_004590a0(CCmdUI* pCmdUI);
	afx_msg void FUN_004588a0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void FUN_00458750(CCmdUI* pCmdUI);
	afx_msg void FUN_00458c80(CCmdUI* pCmdUI);
	afx_msg void FUN_004592c0(CCmdUI* pCmdUI);
	afx_msg void FUN_00459400();
	afx_msg void FUN_00459600(CCmdUI* pCmdUI);
	afx_msg void FUN_00459790(CCmdUI* pCmdUI);
	afx_msg void FUN_00458dc0(CCmdUI* pCmdUI);
	afx_msg void FUN_00458f60(CCmdUI* pCmdUI);
	afx_msg void FUN_0045a0b0(CCmdUI* pCmdUI);
	afx_msg void FUN_0045a600();
	afx_msg void FUN_0045a730();
	afx_msg void FUN_0045a9e0();

	static CString FUN_00454b50(CRobloxDoc* doc, const char* name);

	DECLARE_MESSAGE_MAP()

private:
	friend class CRobloxView;

	std::auto_ptr<XmlElement> m_unk0x60;
	std::vector<RBX::Verb*> m_unk0x64;
	boost::scoped_ptr<RBX::Unknown005d2700> m_unk0x74;

public:
	CComObject<CWorkspace>* m_unk0x78;
};

#endif // ROBLOXDOC_H
