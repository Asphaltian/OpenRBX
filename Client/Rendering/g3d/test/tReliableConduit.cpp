#include "../include/G3DAll.h"

class Message {
public:

	int32			i32;
	int64			i64;
	std::string		s;
	float32			f;

	Message() : i32(iRandom(-10000, 10000)), i64(iRandom(-10000000, 1000000)), f(random(-10000, 10000)) {
		for (int x = 0; x < 20; ++x) {
			s = s + (char)('A' + iRandom(0, 26));
		}
	}

	bool operator==(const Message& m) const {
		return 
			(i32 == m.i32) &&
			(i64 == m.i64) &&
			(s == m.s) &&
			(f == m.f);
	}

	void serialize(BinaryOutput& b) const {
		b.writeInt32(i32);
		b.writeInt64(i64);
		b.writeString(s);
		b.writeFloat32(f);
	}

	void deserialize(BinaryInput& b) {
		i32 = b.readInt32();
		i64 = b.readInt64();
		s   = b.readString();
		f	= b.readFloat32();
	}
};


void testReliableConduit(NetworkDevice* nd) {
	printf("ReliableConduit ");

	debugAssert(nd);

	{
		uint16 port = 10011;
		NetListenerRef listener = nd->createListener(port);

		ReliableConduitRef clientSide = nd->createReliableConduit(NetAddress("localhost", port));
		ReliableConduitRef serverSide = listener->waitForConnection();

		debugAssert(clientSide->ok());
		debugAssert(serverSide->ok());
	
		int type = 10;
		Message a;
		clientSide->send(type, a);

		// Wait for message
		while (!serverSide->waitingMessageType());

		Message b;
		debugAssert(serverSide->waitingMessageType() == type);
		serverSide->receive(b);

		debugAssert(a == b);

		a = Message();
		serverSide->send(type, a);

		// Wait for message
		while (! clientSide->waitingMessageType());
		debugAssert(clientSide->waitingMessageType() == type);
		clientSide->receive(b);

		debugAssert(a == b);

		// Make sure no more messages are waiting
		debugAssert(clientSide->waitingMessageType() == 0);
		debugAssert(serverSide->waitingMessageType() == 0);
	}

	printf("passed\n");
}
