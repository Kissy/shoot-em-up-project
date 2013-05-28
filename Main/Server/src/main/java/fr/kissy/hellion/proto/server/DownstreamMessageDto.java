// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Proto/Server/DownstreamMessage.proto

package fr.kissy.hellion.proto.server;

public final class DownstreamMessageDto {
  private DownstreamMessageDto() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
  }
  public interface DownstreamMessageProtoOrBuilder
      extends com.google.protobuf.MessageOrBuilder {

    // required .DownstreamMessageProto.Type type = 1;
    /**
     * <code>required .DownstreamMessageProto.Type type = 1;</code>
     */
    boolean hasType();
    /**
     * <code>required .DownstreamMessageProto.Type type = 1;</code>
     */
    fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Type getType();

    // optional bytes data = 2;
    /**
     * <code>optional bytes data = 2;</code>
     */
    boolean hasData();
    /**
     * <code>optional bytes data = 2;</code>
     */
    com.google.protobuf.ByteString getData();
  }
  /**
   * Protobuf type {@code DownstreamMessageProto}
   */
  public static final class DownstreamMessageProto extends
      com.google.protobuf.GeneratedMessage
      implements DownstreamMessageProtoOrBuilder {
    // Use DownstreamMessageProto.newBuilder() to construct.
    private DownstreamMessageProto(com.google.protobuf.GeneratedMessage.Builder<?> builder) {
      super(builder);
      this.unknownFields = builder.getUnknownFields();
    }
    private DownstreamMessageProto(boolean noInit) { this.unknownFields = com.google.protobuf.UnknownFieldSet.getDefaultInstance(); }

    private static final DownstreamMessageProto defaultInstance;
    public static DownstreamMessageProto getDefaultInstance() {
      return defaultInstance;
    }

    public DownstreamMessageProto getDefaultInstanceForType() {
      return defaultInstance;
    }

    private final com.google.protobuf.UnknownFieldSet unknownFields;
    @java.lang.Override
    public final com.google.protobuf.UnknownFieldSet
        getUnknownFields() {
      return this.unknownFields;
    }
    private DownstreamMessageProto(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      initFields();
      int mutable_bitField0_ = 0;
      com.google.protobuf.UnknownFieldSet.Builder unknownFields =
          com.google.protobuf.UnknownFieldSet.newBuilder();
      try {
        boolean done = false;
        while (!done) {
          int tag = input.readTag();
          switch (tag) {
            case 0:
              done = true;
              break;
            default: {
              if (!parseUnknownField(input, unknownFields,
                                     extensionRegistry, tag)) {
                done = true;
              }
              break;
            }
            case 8: {
              int rawValue = input.readEnum();
              fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Type value = fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Type.valueOf(rawValue);
              if (value == null) {
                unknownFields.mergeVarintField(1, rawValue);
              } else {
                bitField0_ |= 0x00000001;
                type_ = value;
              }
              break;
            }
            case 18: {
              bitField0_ |= 0x00000002;
              data_ = input.readBytes();
              break;
            }
          }
        }
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        throw e.setUnfinishedMessage(this);
      } catch (java.io.IOException e) {
        throw new com.google.protobuf.InvalidProtocolBufferException(
            e.getMessage()).setUnfinishedMessage(this);
      } finally {
        this.unknownFields = unknownFields.build();
        makeExtensionsImmutable();
      }
    }
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return fr.kissy.hellion.proto.server.DownstreamMessageDto.internal_static_DownstreamMessageProto_descriptor;
    }

    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return fr.kissy.hellion.proto.server.DownstreamMessageDto.internal_static_DownstreamMessageProto_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.class, fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Builder.class);
    }

    public static com.google.protobuf.Parser<DownstreamMessageProto> PARSER =
        new com.google.protobuf.AbstractParser<DownstreamMessageProto>() {
      public DownstreamMessageProto parsePartialFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws com.google.protobuf.InvalidProtocolBufferException {
        return new DownstreamMessageProto(input, extensionRegistry);
      }
    };

    @java.lang.Override
    public com.google.protobuf.Parser<DownstreamMessageProto> getParserForType() {
      return PARSER;
    }

    /**
     * Protobuf enum {@code DownstreamMessageProto.Type}
     */
    public enum Type
        implements com.google.protobuf.ProtocolMessageEnum {
      /**
       * <code>AUTHENTICATE = 1;</code>
       */
      AUTHENTICATE(0, 1),
      /**
       * <code>ENTER_WORLD = 2;</code>
       */
      ENTER_WORLD(1, 2),
      /**
       * <code>PLAYER_MOVE = 3;</code>
       */
      PLAYER_MOVE(2, 3),
      /**
       * <code>PLAYER_SHOT = 4;</code>
       */
      PLAYER_SHOT(3, 4),
      ;

      /**
       * <code>AUTHENTICATE = 1;</code>
       */
      public static final int AUTHENTICATE_VALUE = 1;
      /**
       * <code>ENTER_WORLD = 2;</code>
       */
      public static final int ENTER_WORLD_VALUE = 2;
      /**
       * <code>PLAYER_MOVE = 3;</code>
       */
      public static final int PLAYER_MOVE_VALUE = 3;
      /**
       * <code>PLAYER_SHOT = 4;</code>
       */
      public static final int PLAYER_SHOT_VALUE = 4;


      public final int getNumber() { return value; }

      public static Type valueOf(int value) {
        switch (value) {
          case 1: return AUTHENTICATE;
          case 2: return ENTER_WORLD;
          case 3: return PLAYER_MOVE;
          case 4: return PLAYER_SHOT;
          default: return null;
        }
      }

      public static com.google.protobuf.Internal.EnumLiteMap<Type>
          internalGetValueMap() {
        return internalValueMap;
      }
      private static com.google.protobuf.Internal.EnumLiteMap<Type>
          internalValueMap =
            new com.google.protobuf.Internal.EnumLiteMap<Type>() {
              public Type findValueByNumber(int number) {
                return Type.valueOf(number);
              }
            };

      public final com.google.protobuf.Descriptors.EnumValueDescriptor
          getValueDescriptor() {
        return getDescriptor().getValues().get(index);
      }
      public final com.google.protobuf.Descriptors.EnumDescriptor
          getDescriptorForType() {
        return getDescriptor();
      }
      public static final com.google.protobuf.Descriptors.EnumDescriptor
          getDescriptor() {
        return fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.getDescriptor().getEnumTypes().get(0);
      }

      private static final Type[] VALUES = values();

      public static Type valueOf(
          com.google.protobuf.Descriptors.EnumValueDescriptor desc) {
        if (desc.getType() != getDescriptor()) {
          throw new java.lang.IllegalArgumentException(
            "EnumValueDescriptor is not for this type.");
        }
        return VALUES[desc.getIndex()];
      }

      private final int index;
      private final int value;

      private Type(int index, int value) {
        this.index = index;
        this.value = value;
      }

      // @@protoc_insertion_point(enum_scope:DownstreamMessageProto.Type)
    }

    private int bitField0_;
    // required .DownstreamMessageProto.Type type = 1;
    public static final int TYPE_FIELD_NUMBER = 1;
    private fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Type type_;
    /**
     * <code>required .DownstreamMessageProto.Type type = 1;</code>
     */
    public boolean hasType() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    /**
     * <code>required .DownstreamMessageProto.Type type = 1;</code>
     */
    public fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Type getType() {
      return type_;
    }

    // optional bytes data = 2;
    public static final int DATA_FIELD_NUMBER = 2;
    private com.google.protobuf.ByteString data_;
    /**
     * <code>optional bytes data = 2;</code>
     */
    public boolean hasData() {
      return ((bitField0_ & 0x00000002) == 0x00000002);
    }
    /**
     * <code>optional bytes data = 2;</code>
     */
    public com.google.protobuf.ByteString getData() {
      return data_;
    }

    private void initFields() {
      type_ = fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Type.AUTHENTICATE;
      data_ = com.google.protobuf.ByteString.EMPTY;
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized != -1) return isInitialized == 1;

      if (!hasType()) {
        memoizedIsInitialized = 0;
        return false;
      }
      memoizedIsInitialized = 1;
      return true;
    }

    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        output.writeEnum(1, type_.getNumber());
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        output.writeBytes(2, data_);
      }
      getUnknownFields().writeTo(output);
    }

    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;

      size = 0;
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        size += com.google.protobuf.CodedOutputStream
          .computeEnumSize(1, type_.getNumber());
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        size += com.google.protobuf.CodedOutputStream
          .computeBytesSize(2, data_);
      }
      size += getUnknownFields().getSerializedSize();
      memoizedSerializedSize = size;
      return size;
    }

    private static final long serialVersionUID = 0L;
    @java.lang.Override
    protected java.lang.Object writeReplace()
        throws java.io.ObjectStreamException {
      return super.writeReplace();
    }

    public static fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }
    public static fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input);
    }
    public static fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input, extensionRegistry);
    }
    public static fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }

    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }

    @java.lang.Override
    protected Builder newBuilderForType(
        com.google.protobuf.GeneratedMessage.BuilderParent parent) {
      Builder builder = new Builder(parent);
      return builder;
    }
    /**
     * Protobuf type {@code DownstreamMessageProto}
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder>
       implements fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProtoOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return fr.kissy.hellion.proto.server.DownstreamMessageDto.internal_static_DownstreamMessageProto_descriptor;
      }

      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return fr.kissy.hellion.proto.server.DownstreamMessageDto.internal_static_DownstreamMessageProto_fieldAccessorTable
            .ensureFieldAccessorsInitialized(
                fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.class, fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Builder.class);
      }

      // Construct using fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.newBuilder()
      private Builder() {
        maybeForceBuilderInitialization();
      }

      private Builder(
          com.google.protobuf.GeneratedMessage.BuilderParent parent) {
        super(parent);
        maybeForceBuilderInitialization();
      }
      private void maybeForceBuilderInitialization() {
        if (com.google.protobuf.GeneratedMessage.alwaysUseFieldBuilders) {
        }
      }
      private static Builder create() {
        return new Builder();
      }

      public Builder clear() {
        super.clear();
        type_ = fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Type.AUTHENTICATE;
        bitField0_ = (bitField0_ & ~0x00000001);
        data_ = com.google.protobuf.ByteString.EMPTY;
        bitField0_ = (bitField0_ & ~0x00000002);
        return this;
      }

      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }

      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return fr.kissy.hellion.proto.server.DownstreamMessageDto.internal_static_DownstreamMessageProto_descriptor;
      }

      public fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto getDefaultInstanceForType() {
        return fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.getDefaultInstance();
      }

      public fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto build() {
        fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }

      public fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto buildPartial() {
        fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto result = new fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto(this);
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) == 0x00000001)) {
          to_bitField0_ |= 0x00000001;
        }
        result.type_ = type_;
        if (((from_bitField0_ & 0x00000002) == 0x00000002)) {
          to_bitField0_ |= 0x00000002;
        }
        result.data_ = data_;
        result.bitField0_ = to_bitField0_;
        onBuilt();
        return result;
      }

      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto) {
          return mergeFrom((fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }

      public Builder mergeFrom(fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto other) {
        if (other == fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.getDefaultInstance()) return this;
        if (other.hasType()) {
          setType(other.getType());
        }
        if (other.hasData()) {
          setData(other.getData());
        }
        this.mergeUnknownFields(other.getUnknownFields());
        return this;
      }

      public final boolean isInitialized() {
        if (!hasType()) {
          
          return false;
        }
        return true;
      }

      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto parsedMessage = null;
        try {
          parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
        } catch (com.google.protobuf.InvalidProtocolBufferException e) {
          parsedMessage = (fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto) e.getUnfinishedMessage();
          throw e;
        } finally {
          if (parsedMessage != null) {
            mergeFrom(parsedMessage);
          }
        }
        return this;
      }
      private int bitField0_;

      // required .DownstreamMessageProto.Type type = 1;
      private fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Type type_ = fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Type.AUTHENTICATE;
      /**
       * <code>required .DownstreamMessageProto.Type type = 1;</code>
       */
      public boolean hasType() {
        return ((bitField0_ & 0x00000001) == 0x00000001);
      }
      /**
       * <code>required .DownstreamMessageProto.Type type = 1;</code>
       */
      public fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Type getType() {
        return type_;
      }
      /**
       * <code>required .DownstreamMessageProto.Type type = 1;</code>
       */
      public Builder setType(fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Type value) {
        if (value == null) {
          throw new NullPointerException();
        }
        bitField0_ |= 0x00000001;
        type_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>required .DownstreamMessageProto.Type type = 1;</code>
       */
      public Builder clearType() {
        bitField0_ = (bitField0_ & ~0x00000001);
        type_ = fr.kissy.hellion.proto.server.DownstreamMessageDto.DownstreamMessageProto.Type.AUTHENTICATE;
        onChanged();
        return this;
      }

      // optional bytes data = 2;
      private com.google.protobuf.ByteString data_ = com.google.protobuf.ByteString.EMPTY;
      /**
       * <code>optional bytes data = 2;</code>
       */
      public boolean hasData() {
        return ((bitField0_ & 0x00000002) == 0x00000002);
      }
      /**
       * <code>optional bytes data = 2;</code>
       */
      public com.google.protobuf.ByteString getData() {
        return data_;
      }
      /**
       * <code>optional bytes data = 2;</code>
       */
      public Builder setData(com.google.protobuf.ByteString value) {
        if (value == null) {
    throw new NullPointerException();
  }
  bitField0_ |= 0x00000002;
        data_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>optional bytes data = 2;</code>
       */
      public Builder clearData() {
        bitField0_ = (bitField0_ & ~0x00000002);
        data_ = getDefaultInstance().getData();
        onChanged();
        return this;
      }

      // @@protoc_insertion_point(builder_scope:DownstreamMessageProto)
    }

    static {
      defaultInstance = new DownstreamMessageProto(true);
      defaultInstance.initFields();
    }

    // @@protoc_insertion_point(class_scope:DownstreamMessageProto)
  }

  private static com.google.protobuf.Descriptors.Descriptor
    internal_static_DownstreamMessageProto_descriptor;
  private static
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_DownstreamMessageProto_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n$Proto/Server/DownstreamMessage.proto\"\237" +
      "\001\n\026DownstreamMessageProto\022*\n\004type\030\001 \002(\0162" +
      "\034.DownstreamMessageProto.Type\022\014\n\004data\030\002 " +
      "\001(\014\"K\n\004Type\022\020\n\014AUTHENTICATE\020\001\022\017\n\013ENTER_W" +
      "ORLD\020\002\022\017\n\013PLAYER_MOVE\020\003\022\017\n\013PLAYER_SHOT\020\004" +
      "B8\n\035fr.kissy.hellion.proto.serverB\024Downs" +
      "treamMessageDto\210\001\000"
    };
    com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner assigner =
      new com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner() {
        public com.google.protobuf.ExtensionRegistry assignDescriptors(
            com.google.protobuf.Descriptors.FileDescriptor root) {
          descriptor = root;
          internal_static_DownstreamMessageProto_descriptor =
            getDescriptor().getMessageTypes().get(0);
          internal_static_DownstreamMessageProto_fieldAccessorTable = new
            com.google.protobuf.GeneratedMessage.FieldAccessorTable(
              internal_static_DownstreamMessageProto_descriptor,
              new java.lang.String[] { "Type", "Data", });
          return null;
        }
      };
    com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
        }, assigner);
  }

  // @@protoc_insertion_point(outer_class_scope)
}
