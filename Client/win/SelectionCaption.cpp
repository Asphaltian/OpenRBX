#include "SelectionCaption.h"

IMPLEMENT_DYNAMIC(CSelectionCaption, CStatic)

BEGIN_MESSAGE_MAP(CSelectionCaption, CStatic)
ON_WM_CREATE()
END_MESSAGE_MAP()

// FUNCTION: ROBLOX 0x0046a240
BOOL CSelectionCaption::PreCreateWindow(CREATESTRUCT& cs)
{
	return CXTCaption::PreCreateWindow(cs);
}

// FUNCTION: ROBLOX 0x0046a250
int CSelectionCaption::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTCaption::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}
	FUN_004204e0();
	return 0;
}

// STUB: ROBLOX 0x0046a290
void CSelectionCaption::FUN_0046a290()
{
	if (m_hWnd) {
		CString caption;
		std::string name;
		if (m_unk0x1b8) {
			RBX::DataModel::Lock lock(m_unk0x1b0);
			std::vector<boost::shared_ptr<RBX::Instance> >::const_iterator it = m_unk0x1b8->begin();
			if (it != m_unk0x1b8->end()) {
				name = (*it)->getName();
				const RBX::Name* className = &(*it)->getClassName();
				while (++it != m_unk0x1b8->end()) {
					if ((*it)->getName() != name) {
						name = "";
						if (!className) {
							break;
						}
					}
					if (&(*it)->getClassName() != className) {
						className = NULL;
						if (name == "") {
							break;
						}
					}
				}

				if (!className) {
					caption.Format(183, m_unk0x1b8->size());
				}
				else if (name != "") {
					if (m_unk0x1b8->size() > 1) {
						caption.Format(181, className->name.c_str(), name.c_str(), m_unk0x1b8->size());
					}
					else {
						caption.Format(180, className->name.c_str(), name.c_str());
					}
				}
				else {
					caption.Format(182, className->name.c_str(), m_unk0x1b8->size());
				}
			}
		}
		UpdateCaption(caption, NULL);
	}
}

// FUNCTION: ROBLOX 0x0046a920
CSelectionCaption::~CSelectionCaption()
{
	RBX::DataModel::Lock lock(m_unk0x1b0);
	RBX::stopListening<RBX::Selection, RBX::SelectionChanged>(m_unk0x1b8, this);
}

// FUNCTION: ROBLOX 0x0046aa90
bool CSelectionCaption::VTable0x10(
	const RBX::Selection* source,
	const RBX::SelectionChanged& event,
	const RBX::Selection* oldSource,
	const RBX::SelectionChanged& oldEvent
)
{
	return true;
}

// FUNCTION: ROBLOX 0x0046aad0
void CSelectionCaption::VTable0x08(const RBX::Selection* source, RBX::SelectionChanged event)
{
	FUN_0046a290();
}

// FUNCTION: ROBLOX 0x0046ab30
CSelectionCaption::CSelectionCaption()
{
}

// FUNCTION: ROBLOX 0x0046adc0
void CSelectionCaption::VTable0x00(
	boost::shared_ptr<RBX::DataModel> newDataModel, boost::shared_ptr<RBX::DataModel> oldDataModel
)
{
	{
		RBX::DataModel::Lock lock(m_unk0x1b0);
		RBX::stopListening<RBX::Selection, RBX::SelectionChanged>(m_unk0x1b8, this);
		m_unk0x1b8.reset();
	}

	m_unk0x1b0.reset();

	if (newDataModel) {
		m_unk0x1b0 = newDataModel;

		RBX::DataModel::Lock lock(m_unk0x1b0);
		m_unk0x1b8 = RBX::shared_from(m_unk0x1b0 ? m_unk0x1b0->create<RBX::Selection>() : NULL);
		RBX::startListening<RBX::Selection, RBX::SelectionChanged>(m_unk0x1b8, this);
	}

	if (!m_unk0x1b8) {
		FUN_0046a290();
	}
}

template class RBX::MarshaledListener<RBX::Selection, RBX::SelectionChanged, RBX::Selection, RBX::SelectionChanged>;
template class RBX::FilteredListener<RBX::Selection, RBX::SelectionChanged, RBX::Selection, RBX::SelectionChanged>;
