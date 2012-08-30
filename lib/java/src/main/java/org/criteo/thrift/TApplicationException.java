/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package org.criteo.thrift;

import org.criteo.thrift.protocol.TField;
import org.criteo.thrift.protocol.TProtocol;
import org.criteo.thrift.protocol.TProtocolUtil;
import org.criteo.thrift.protocol.TStruct;
import org.criteo.thrift.protocol.TType;

/**
 * Application level exception
 *
 */
public class TApplicationException extends TException {

  private static final TStruct TAPPLICATION_EXCEPTION_STRUCT = new TStruct("TApplicationException");
  private static final TField MESSAGE_FIELD = new TField("message", TType.STRING, (short)1);
  private static final TField TYPE_FIELD = new TField("type", TType.I32, (short)2);

  private static final long serialVersionUID = 1L;

  public static final int UNKNOWN = 0;
  public static final int UNKNOWN_METHOD = 1;
  public static final int INVALID_MESSAGE_TYPE = 2;
  public static final int WRONG_METHOD_NAME = 3;
  public static final int BAD_SEQUENCE_ID = 4;
  public static final int MISSING_RESULT = 5;
  public static final int INTERNAL_ERROR = 6;
  public static final int PROTOCOL_ERROR = 7;

  protected int type_ = UNKNOWN;

  public TApplicationException() {
    super();
  }

  public TApplicationException(int type) {
    super();
    type_ = type;
  }

  public TApplicationException(int type, String message) {
    super(message);
    type_ = type;
  }

  public TApplicationException(String message) {
    super(message);
  }

  public int getType() {
    return type_;
  }

  public static TApplicationException read(TProtocol iprot) throws TException {
    TField field;
    iprot.readStructBegin();

    String message = null;
    int type = UNKNOWN;

    while (true) {
      field = iprot.readFieldBegin();
      if (field.type == TType.STOP) {
        break;
      }
      switch (field.id) {
      case 1:
        if (field.type == TType.STRING) {
          message = iprot.readString();
        } else {
          TProtocolUtil.skip(iprot, field.type);
        }
        break;
      case 2:
        if (field.type == TType.I32) {
          type = iprot.readI32();
        } else {
          TProtocolUtil.skip(iprot, field.type);
        }
        break;
      default:
        TProtocolUtil.skip(iprot, field.type);
        break;
      }
      iprot.readFieldEnd();
    }
    iprot.readStructEnd();

    return new TApplicationException(type, message);
  }

  public void write(TProtocol oprot) throws TException {
    oprot.writeStructBegin(TAPPLICATION_EXCEPTION_STRUCT);
    if (getMessage() != null) {
      oprot.writeFieldBegin(MESSAGE_FIELD);
      oprot.writeString(getMessage());
      oprot.writeFieldEnd();
    }
    oprot.writeFieldBegin(TYPE_FIELD);
    oprot.writeI32(type_);
    oprot.writeFieldEnd();
    oprot.writeFieldStop();
    oprot.writeStructEnd();
  }
}