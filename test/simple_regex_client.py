#!/usr/bin/python
# -*- coding: utf-8 -*-

from scribe import scribe
from thrift.transport import TTransport, TSocket
from thrift.protocol import TBinaryProtocol

categories = ['Foo','ABCD','heLLO','bar']
log_entry = []

for x in categories :
  log_entry.append(scribe.LogEntry(x, message='hello world'))


socket = TSocket.TSocket(host='127.0.0.1', port=1463)
transport = TTransport.TFramedTransport(socket)
protocol = TBinaryProtocol.TBinaryProtocol(trans=transport, strictRead=False, strictWrite=False)
client = scribe.Client(iprot=protocol, oprot=protocol)

transport.open()
result = client.Log(log_entry)
transport.close()
 
