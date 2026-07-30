#ifndef __FILE_LIST_TRANSFER_CALLBACK_INTERFACE_H
#define __FILE_LIST_TRANSFER_CALLBACK_INTERFACE_H

#ifdef _MSC_VER
#pragma warning( push )
#endif

/// \brief Used by FileListTransfer plugin as a callback for when we get a file.
/// You get the last file when fileIndex==setCount
/// \sa FileListTransfer
class FileListTransferCBInterface
{
public:
	// Got a file
	virtual void OnFile(
		unsigned fileIndex,
		char *filename,
		char *fileData,
		unsigned compressedTransmissionLength,
		unsigned finalDataLength,
		unsigned short setID,
		unsigned setCount,	
		unsigned setTotalCompressedTransmissionLength,
		unsigned setTotalFinalLength,
		unsigned char context)=0;

#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100 : unreferenced formal parameter
#endif
	// Got part of a big file.  Activate with RakPeer::SetSplitMessageProgressInterval
	virtual void OnFileProgress(unsigned fileIndex,
		char *filename,
		unsigned compressedTransmissionLength,
		unsigned finalDataLength,
		unsigned short setID,
		unsigned setCount,	
		unsigned setTotalCompressedTransmissionLength,
		unsigned setTotalFinalLength,
		unsigned char context,
		unsigned int partCount,
		unsigned int partTotal,
		unsigned int partLength) {}
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif

