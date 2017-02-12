using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Xml.Serialization;


class Serialisierer {
    public static String serialize(Type serialisedObjectType, object obj) {
        XmlSerializer serializer = new XmlSerializer(serialisedObjectType);
        MemoryStream memoryStream = new MemoryStream();
        serializer.Serialize(memoryStream, obj);
        memoryStream.Position = 0;
        return new StreamReader(memoryStream).ReadToEnd();
    }
    public static byte[] serialize(object obj) {
        using ( MemoryStream ms = new MemoryStream() ) {
            new BinaryFormatter().Serialize(ms, obj);
            return ms.ToArray();
        }
    }
}
class Deserialisierer {
    public static object deserialize(byte[] data) {
        using ( MemoryStream ms = new MemoryStream() ) {
            ms.Write(data, 0, data.Length);
            ms.Seek(0, SeekOrigin.Begin);
            return new BinaryFormatter().Deserialize(ms);
        }
    }
    public static object deserialize(Type deserialisedObjectType, String xml) {
        XmlSerializer deserializer = new XmlSerializer(deserialisedObjectType);
        MemoryStream memoryStream = new MemoryStream();
        StreamWriter streamWriter = new StreamWriter(memoryStream);
        streamWriter.Write(xml);
        streamWriter.Flush();
        memoryStream.Position = 0;
        return deserializer.Deserialize(memoryStream);
        ;
    }
}

