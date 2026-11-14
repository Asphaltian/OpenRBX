#ifndef CLIPBOARDVERBS_H
#define CLIPBOARDVERBS_H

#include "placeholder.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/DataModel.h"
#include "v8tree/Verb.h"

class PasteVerb : public RBX::Verb
{
public:
	PasteVerb(RBX::DataModel* dataModel, bool pasteInto);

	virtual bool isEnabled() const;
	virtual void doIt(RBX::IDataState* dataState);

private:
	RBX::DataModel* m_unk0x0c;
	bool m_unk0x10;
};

class CutVerb : public RBX::DeleteSelectionVerb
{
public:
	CutVerb(RBX::DataModel* dataModel) : RBX::DeleteSelectionVerb(dataModel, dataModel, "Cut") {}

	virtual void doIt(RBX::IDataState* dataState);
};

class CopyVerb : public RBX::EditSelectionVerb
{
public:
	CopyVerb(RBX::DataModel* dataModel);

	virtual ~CopyVerb() {}

	virtual void doIt(RBX::IDataState* dataState);
};

#endif // CLIPBOARDVERBS_H
