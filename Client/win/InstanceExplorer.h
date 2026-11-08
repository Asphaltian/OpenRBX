#ifndef INSTANCEEXPLORER_H
#define INSTANCEEXPLORER_H

#include "MarshaledListener.h"
#include "decomp.h"
#include "v8datamodel/Commands.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>

// SIZE 0x5c
// VTABLE: ROBLOX 0x00801bfc
// SIZE 0x24
class InsertDecal : public RBX::EditSelectionVerb
{
public:
	InsertDecal(RBX::DataModel* dataModel, const char* name);

	virtual bool isEnabled() const;
	virtual void doIt(RBX::IDataState* dataState);

private:
	RBX::Instance* FUN_0041f500() const;
};

DECOMP_SIZE_ASSERT(InsertDecal, 0x24)

// SIZE 0x24
class InsertTexture : public InsertDecal
{
public:
	InsertTexture(RBX::DataModel* dataModel) : InsertDecal(dataModel, "InsertTexture") {}
};

DECOMP_SIZE_ASSERT(InsertTexture, 0x24)

// VTABLE: ROBLOX 0x00801c94
// SIZE 0x1c
class InsertObject : public RBX::RunStateVerb
{
public:
	InsertObject(RBX::DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(RBX::IDataState* dataState);

private:
	RBX::Instance* FUN_0041fb40() const;
};

DECOMP_SIZE_ASSERT(InsertObject, 0x1c)

class CInstanceRecord : public CXTPReportRecord
{
public:
	// SIZE 0x80
	class CNameItem : public CXTPReportRecordItem
	{
	public:
		CNameItem(RBX::Instance* instance) : m_unk0x7c(instance) {}

		virtual CString GetCaption(CXTPReportColumn* pColumn);

	private:
		RBX::Instance* m_unk0x7c; // 0x7c
	};

	CInstanceRecord(boost::shared_ptr<RBX::Instance> instance);
	virtual ~CInstanceRecord();

	// SYNTHETIC: ROBLOX 0x00420880
	// CInstanceRecord::`scalar deleting destructor'

	boost::shared_ptr<RBX::Instance> m_unk0x54; // 0x54
};

DECOMP_SIZE_ASSERT(CInstanceRecord::CNameItem, 0x80)
DECOMP_SIZE_ASSERT(CInstanceRecord, 0x5c)

// SIZE 0x3b0
class CInstanceExplorer
	: public CXTPReportView,
	  public RBX::MarshaledListener<RBX::Instance, RBX::DescendentAdded, RBX::Instance, RBX::DescendentAdded>,
	  public RBX::MarshaledListener<RBX::Instance, RBX::DescendentRemoving, RBX::Instance, RBX::DescendentRemoving>
{
	DECLARE_DYNCREATE(CInstanceExplorer)

	// SYNTHETIC: ROBLOX 0x00420030
	// ?_GetBaseClass@CInstanceExplorer@@KGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x00420040
	// ?GetThisClass@CInstanceExplorer@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x00421120
	// ?CreateObject@CInstanceExplorer@@SGPAVCObject@@XZ

protected:
	CInstanceExplorer();

public:
	virtual ~CInstanceExplorer();

	// SYNTHETIC: ROBLOX 0x00421510
	// CInstanceExplorer::`scalar deleting destructor'

	void FUN_004212d0(RBX::Instance* root);

protected:
	virtual void VTable0x08(const RBX::Instance* source, RBX::DescendentRemoving event);
	virtual void VTable0x08(const RBX::Instance* source, RBX::DescendentAdded event);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void FUN_00421580(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x004216c0
	// ?GetMessageMap@CInstanceExplorer@@MBEPBUAFX_MSGMAP@@XZ

private:
	boost::shared_ptr<RBX::Instance> m_unk0x3a4; // 0x3a4
	bool m_unk0x3ac;                             // 0x3ac
};

DECOMP_SIZE_ASSERT(CInstanceExplorer, 0x3b0)

HRESULT FUN_0041e610(LPCTSTR url);
HRESULT FUN_0041e630(BSTR url);

#endif // INSTANCEEXPLORER_H
