#include "ClipboardVerbs.h"

#include "v8datamodel/Selection.h"
#include "v8datamodel/Workspace.h"
#include "v8xml/SerializerV2.h"
#include "v8xml/XmlSerializer.h"

#include <boost/bind.hpp>
#include <sstream>

template <class T>
static T* createService(RBX::DataModel* dataModel)
{
	return dataModel ? dataModel->create<T>() : NULL;
}

static UINT clipboardFormat()
{
	static UINT format = RegisterClipboardFormat("Roblox");

	return format;
}

void CutVerb::doIt(RBX::IDataState* dataState)
{
	getContainer()->getVerb("Copy")->doIt(dataState);
	RBX::DeleteBase::doIt(dataState);
}

PasteVerb::PasteVerb(RBX::DataModel* dataModel, bool pasteInto)
	: RBX::Verb(dataModel, pasteInto ? "PasteInto" : "Paste"), m_unk0x0c(dataModel), m_unk0x10(pasteInto)
{
}

CopyVerb::CopyVerb(RBX::DataModel* dataModel) : RBX::EditSelectionVerb("Copy", dataModel)
{
}

bool PasteVerb::isEnabled() const
{
	if (IsClipboardFormatAvailable(clipboardFormat()) != TRUE) {
		return false;
	}

	if (!m_unk0x10) {
		return true;
	}

	return (m_unk0x0c ? m_unk0x0c->create<RBX::Selection>() : NULL)->size() == 1;
}

void PasteVerb::doIt(RBX::IDataState* dataState)
{
	if (m_unk0x0c->findFirstChildByName("NetworkClient")) {
		return;
	}

	if (!AfxGetMainWnd()->OpenClipboard()) {
		return;
	}

	HANDLE handle = GetClipboardData(clipboardFormat());
	const char* text = static_cast<const char*>(GlobalLock(handle));
	std::vector<boost::shared_ptr<RBX::Instance> > instances;
	{
		std::istringstream stream(text);
		TextXmlParser machine(stream.rdbuf());
		std::auto_ptr<XmlElement> root(machine.parse());

		RBX::Selection* selection = createService<RBX::Selection>(m_unk0x0c);
		RBX::Instance* parent =
			m_unk0x10 && selection->size() == 1 ? selection->front().get() : m_unk0x0c->getWorkspace();
		RBX::InsertMode insertMode = m_unk0x10 ? RBX::INSERT_TO_TREE : RBX::INSERT_TO_3D_VIEW;
		SerializerV2 serializer;
		serializer.loadInstances(root.get(), instances);
		m_unk0x0c->getWorkspace()->insertInstances(instances, parent, insertMode, RBX::SUPPRESS_PROMPTS);

		const std::vector<boost::shared_ptr<RBX::Instance> >& items = instances;
		selection->setSelection(items.begin(), items.end());
	}

	GlobalUnlock(handle);
	CloseClipboard();

	RBX::XmlState* undoState = new RBX::UndoState();
	RBX::XmlState* redoState = new RBX::RedoState();
	for (std::vector<boost::shared_ptr<RBX::Instance> >::iterator iter = instances.begin(); iter != instances.end();
		 ++iter) {
		undoState->addDelete(*iter->get());
		redoState->addParentProperty(*iter->get());
	}

	dataState->pushState(getName(), undoState, redoState);
	dataState->setDirty(true);
}

void CopyVerb::doIt(RBX::IDataState* dataState)
{
	XmlElement* root = SerializerV2::newRootElement();
	RBX::Selection* selection = createService<RBX::Selection>(dataModel);
	std::for_each(selection->begin(), selection->end(), boost::bind(&RBX::FUN_00597490, root, _1));

	std::ostringstream stream;
	TextXmlWriter writer(stream);
	writer.serialize(root);
	delete root;

	if (AfxGetMainWnd()->OpenClipboard()) {
		EmptyClipboard();

		std::string text = stream.str();
		HGLOBAL handle = GlobalAlloc(GMEM_MOVEABLE, text.length() + 1);
		char* buffer = static_cast<char*>(GlobalLock(handle));
		strcpy(buffer, text.c_str());
		GlobalUnlock(handle);

		SetClipboardData(clipboardFormat(), handle);
		CloseClipboard();
	}
}
