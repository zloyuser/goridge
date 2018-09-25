package goridge

import "sync"

var bytesPool = sync.Pool{
	New: func() interface{} {
		return []byte{}
	},
}

func makeBytes() []byte {
	return bytesPool.Get().([]byte)
}

func releaseBytes(b []byte) {
	//reset
	b = b[:0]
	bytesPool.Put(b)
}
