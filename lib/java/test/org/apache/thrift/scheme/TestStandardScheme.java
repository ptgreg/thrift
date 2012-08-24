package org.criteo.thrift.scheme;

import junit.framework.TestCase;

import org.criteo.thrift.Fixtures;
import org.criteo.thrift.TBase;
import org.criteo.thrift.TDeserializer;
import org.criteo.thrift.TException;
import org.criteo.thrift.TSerializer;
import org.criteo.thrift.protocol.TBinaryProtocol;
import org.criteo.thrift.protocol.TProtocol;
import org.criteo.thrift.transport.TMemoryBuffer;
import org.criteo.thrift.transport.TTransport;

import thrift.test.HolyMoley;
import thrift.test.Nesting;
import thrift.test.OneOfEach;

public class TestStandardScheme extends TestCase {
  TSerializer serializer = new TSerializer();
  TDeserializer deserializer = new TDeserializer();

  /**
   * This tests whether the Standard Scheme properly reads structs serialized
   * using an older version of thrift.
   */
  public void testPersistentStructs() throws TException {
    readAndCompare(new OneOfEach(), Fixtures.oneOfEach, Fixtures.persistentBytesOneOfEach);
    readAndCompare(new HolyMoley(), Fixtures.holyMoley, Fixtures.persistentBytesHolyMoley);
    readAndCompare(new Nesting(), Fixtures.nesting, Fixtures.persistentBytesNesting);
  }

  public void readAndCompare(TBase struct, TBase fixture, byte[] inputBytes) throws TException {
    TTransport trans = new TMemoryBuffer(0);
    trans.write(inputBytes, 0, inputBytes.length);
    TProtocol iprot = new TBinaryProtocol(trans);
    struct.read(iprot);
    assertEquals(fixture, struct);
  }
}
