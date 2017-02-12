#ifndef OSIRISIINTERNSTRUCTS_HPP_
#define OSIRISIINTERNSTRUCTS_HPP_

#include <math.h>
#include <string.h>

namespace OsirisI {
	typedef long ReturnState;
	typedef enum OsirisI_DeviceType { DX11 = 0, OpenGL };

	enum class MouseButtons { Left, Right, Middle, None };
	enum class MouseActions { Up, Down, DoubleClick, Move, None };
	enum class KeyboardKeys { LCtrl = 0, RCtrl, LAlt, RAlt, AltGr, RShift, LShift, ESC, Space, Backspace, Home, End, Insert, Delete, PageUp, PageDown, Return, ArrowLeft, ArrowRight, ArrowUp, ArrowDown, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, TOP0, TOP1, TOP2, TOP3, TOP4, TOP5, TOP6, TOP7, TOP8, TOP9, NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, None };
	static const char* KeyboardKeysNames[] = { "LCtrl", "RCtrl", "LAlt", "RAlt", "AltGr", "RShift", "LShift", "ESC", "Space", "Backspace", "Home", "End", "Insert", "Delete", "PageUp", "PageDown", "Return", "ArrowLeft", "ArrowRight", "ArrowUp", "ArrowDown", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "TOP0", "TOP1", "TOP2", "TOP3", "TOP4", "TOP5", "TOP6", "TOP7", "TOP8", "TOP9", "NUM0", "NUM1", "NUM2", "NUM3", "NUM4", "NUM5", "NUM6", "NUM7", "NUM8", "NUM9", "None" };
	static const char* KeyboardKeysValues[] = { "LCtrl", "RCtrl", "LAlt", "RAlt", "AltGr", "RShift", "LShift", "ESC", "Space", "Backspace", "Home", "End", "Insert", "Delete", "PageUp", "PageDown", "Return", "ArrowLeft", "ArrowRight", "ArrowUp", "ArrowDown", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "None" };

	struct KeyboardKeysHelper {
	public:
		static void ListEntries() {
			for (int i = 0; i < (int)KeyboardKeys::None; i++) {
				std::cout << "[" << i << "]" << KeyboardKeysNames[i] << std::endl;
			}
		}
	};

    struct UV_Coordinates {
        float LeftUpU, RightDownU, LeftUpV, RightDownV, TexLength, TexHeight;

        UV_Coordinates() {
            LeftUpU = 0;
            LeftUpV = 0;
            RightDownU = 0;
            RightDownV = 0;
            TexLength = 0;
            TexHeight = 0;
        }

        UV_Coordinates(float LeftUpU, float RightDownU, float LeftUpV, float RightDownV, float TexLength, float TexHeight) {
            this->LeftUpU = LeftUpU;
            this->LeftUpV = LeftUpV;
            this->RightDownU = RightDownU;
            this->RightDownV = RightDownV;
            this->TexLength = TexLength;
            this->TexHeight = TexHeight;
        }
    };
    struct Rectangle {
        float x, y, width, height;

        Rectangle() {
            x = 0;
            y = 0;
            width = 0;
            height = 0;
        }

        Rectangle(float x, float y, float width, float height) {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
        }

        Rectangle(Rectangle& rhs) {
            this->x = rhs.x;
            this->y = rhs.y;
            this->width = rhs.width;
            this->height = rhs.height;
        }
    };

	struct OVector2 {
        float x, y;

        OVector2() {
            x = 0;
            y = 0;
        }

        OVector2(OVector2 &rhs) {
            x = rhs.x;
            y = rhs.y;
        }

        OVector2(float x, float y) {
            this->x = x;
            this->y = y;
        }

        float length() {
            return sqrt(abs(x) + abs(y));
        }

        OVector2 operator+ (OVector2 const& rhs) {
            return OVector2(x + rhs.x, y + rhs.y);
        }
        OVector2 operator- (OVector2 const& rhs) {
            return OVector2(x - rhs.x, y - rhs.y);
        }
        OVector2 operator* (float r) {
            return OVector2(x * r, y * r);
        }
        OVector2 operator/ (float r) {
            return OVector2(x / r, y / r);
        }
        /** Scalarproduct.
            This operator provides the scalarproduct of two vectors.
            */
        float operator*(OVector2 const& rhs) {
            return (x * rhs.x) + (y * rhs.y);
        }

        OVector2& operator+=(OVector2 const& rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }
        OVector2& operator-=(OVector2 const& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }
        OVector2& operator*=(float r) {
            x *= r;
            y *= r;
            return *this;
        }
        OVector2& operator/=(float r) {
            x /= r;
            y /= r;
            return *this;
        }
        bool operator==(OVector2 const& rhs) {
            if(rhs.x == this->x && rhs.y == this->y) return true;
            else return false;
        }
    };

    struct OVector3 {
        float x, y, z;

        OVector3() {
            x = 0;
            y = 0;
            z = 0;
        }

        OVector3(OVector3& rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
        }
        OVector3(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        float length() {
            return sqrt(abs(x) + abs(y) + abs(z));
        }

        OVector3 CrossProduct(OVector3 const& rhs) {
            return OVector3(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
        }

        OVector3 operator+ (OVector3 const& rhs) {
            return OVector3(x + rhs.x, y + rhs.y, z + rhs.z);
        }
        OVector3 operator- (OVector3 const& rhs) {
            return OVector3(x - rhs.x, y - rhs.y, z - rhs.z);
        }
        OVector3 operator* (float r) {
            return OVector3(x * r, y * r, z * r);
        }
        OVector3 operator/ (float r) {
            return OVector3(x / r, y / r, z / r);
        }
        /** Scalarproduct.
            This operator provides the scalarproduct of two vectors.
            */
        float operator*(OVector3 const& rhs) {
            return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
        }

        OVector3& operator+=(OVector3 const& rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }
        OVector3& operator-=(OVector3 const& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
        OVector3& operator*=(float r) {
            x *= r;
            y *= r;
            z *= r;
            return *this;
        }
        OVector3& operator/=(float r) {
            x /= r;
            y /= r;
            z /= r;
            return *this;
        }
    };
    struct OVector4 {
        float x, y, z, w;
        OVector4() {
            x = 0;
            y = 0;
            z = 0;
            w = 0;
        }
        OVector4(OVector4& rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;
        }
        OVector4(float x, float y, float z, float w) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        float length() {
            return sqrt(abs(x) + abs(y) + abs(z) + abs(w));
        }

        OVector4 operator+ (OVector4 const& rhs) {
            return OVector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }
        OVector4 operator- (OVector4 const& rhs) {
            return OVector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }
        OVector4 operator* (float r) {
            return OVector4(x * r, y * r, z * r, w * r);
        }
        OVector4 operator/ (float r) {
            return OVector4(x / r, y / r, z / r, w / r);
        }
        /** Scalarproduct.
            This operator provides the scalarproduct of two vectors.
            */
        float operator*(OVector4 const& rhs) {
            return (x * rhs.x) + (y * rhs.y) + (z * rhs.z) + (w * rhs.w);
        }

        OVector4& operator+=(OVector4 const& rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }
        OVector4& operator-=(OVector4 const& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
            return *this;
        }
        OVector4& operator*=(float r) {
            x *= r;
            y *= r;
            z *= r;
            w *= r;
            return *this;
        }
        OVector4& operator/=(float r) {
            x /= r;
            y /= r;
            z /= r;
            w /= r;
            return *this;
        }
    };
    struct OMatrix4x4 {
        float f[16];

        OMatrix4x4() {
            memset(&f, 0, 16 * sizeof(float));
        }
        OMatrix4x4(float fr[16]) {
            memcpy(&f, &fr, 16 * sizeof(float));
        }
        OMatrix4x4(float fr[4][4]) {
            memcpy(&f, fr, 16 * sizeof(float));
        }
        OMatrix4x4(float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15) {
            this->f[0] = f0;
            this->f[1] = f1;
            this->f[2] = f2;
            this->f[3] = f3;
            this->f[4] = f4;
            this->f[5] = f5;
            this->f[6] = f6;
            this->f[7] = f7;
            this->f[8] = f8;
            this->f[9] = f9;
            this->f[10] = f10;
            this->f[11] = f11;
            this->f[12] = f12;
            this->f[13] = f13;
            this->f[14] = f14;
            this->f[15] = f15;
        }
        OMatrix4x4(OMatrix4x4& rhs) {
            memcpy(&f, &rhs.f, 16 * sizeof(float));
        }

        OMatrix4x4 operator+ (OMatrix4x4 const& rhs) {
            return OMatrix4x4(f[0] + rhs.f[0],
                f[1] + rhs.f[1],
                f[2] + rhs.f[2],
                f[3] + rhs.f[3],
                f[4] + rhs.f[4],
                f[5] + rhs.f[5],
                f[6] + rhs.f[6],
                f[7] + rhs.f[7],
                f[8] + rhs.f[8],
                f[9] + rhs.f[9],
                f[10] + rhs.f[10],
                f[11] + rhs.f[11],
                f[12] + rhs.f[12],
                f[13] + rhs.f[13],
                f[14] + rhs.f[14],
                f[15] + rhs.f[15]);
        }
        OMatrix4x4 operator- (OMatrix4x4 const& rhs) {
            return OMatrix4x4(f[0] - rhs.f[0], f[1] - rhs.f[1], f[2] - rhs.f[2], f[3] - rhs.f[3], f[4] - rhs.f[4], f[5] - rhs.f[5], f[6] - rhs.f[6], f[7] - rhs.f[7], f[8] - rhs.f[8], f[9] - rhs.f[9], f[10] - rhs.f[10], f[11] - rhs.f[11], f[12] - rhs.f[12], f[13] - rhs.f[13], f[14] - rhs.f[14], f[15] - rhs.f[15]);
        }
        OMatrix4x4 operator* (OMatrix4x4 const& rhs) {
            float rf[16];
            memcpy(&rf, &rhs.f, 16 * sizeof(float));

            float f0_0 = f[0] * rf[0] + f[1] * rf[4] + f[2] * rf[8] + f[3] * rf[12];
            float f0_1 = f[0] * rf[1] + f[1] * rf[5] + f[2] * rf[9] + f[3] * rf[13];
            float f0_2 = f[0] * rf[2] + f[1] * rf[6] + f[2] * rf[10] + f[3] * rf[14];
            float f0_3 = f[0] * rf[3] + f[1] * rf[7] + f[2] * rf[11] + f[3] * rf[15];

            float f1_0 = f[4] * rf[0] + f[5] * rf[4] + f[6] * rf[8] + f[7] * rf[12];
            float f1_1 = f[4] * rf[1] + f[5] * rf[5] + f[6] * rf[9] + f[7] * rf[13];
            float f1_2 = f[4] * rf[2] + f[5] * rf[6] + f[6] * rf[10] + f[7] * rf[14];
            float f1_3 = f[4] * rf[3] + f[5] * rf[7] + f[6] * rf[11] + f[7] * rf[15];

            float f2_0 = f[8] * rf[0] + f[9] * rf[4] + f[10] * rf[8] + f[11] * rf[12];
            float f2_1 = f[8] * rf[1] + f[9] * rf[5] + f[10] * rf[9] + f[11] * rf[13];
            float f2_2 = f[8] * rf[2] + f[9] * rf[6] + f[10] * rf[10] + f[11] * rf[14];
            float f2_3 = f[8] * rf[3] + f[9] * rf[7] + f[10] * rf[11] + f[11] * rf[15];

            float f3_0 = f[12] * rf[0] + f[13] * rf[4] + f[14] * rf[8] + f[15] * rf[12];
            float f3_1 = f[12] * rf[1] + f[13] * rf[5] + f[14] * rf[9] + f[15] * rf[13];
            float f3_2 = f[12] * rf[2] + f[13] * rf[6] + f[14] * rf[10] + f[15] * rf[14];
            float f3_3 = f[12] * rf[3] + f[13] * rf[7] + f[14] * rf[11] + f[15] * rf[15];

            return OMatrix4x4(f0_0, f0_1, f0_2, f0_3, f1_0, f1_1, f1_2, f1_3, f2_0, f2_1, f2_2, f2_3, f3_0, f3_1, f3_2, f3_3);
        }
        OMatrix4x4 operator* (float r) {
            return OMatrix4x4(r*f[0], r*f[1], r*f[2], r*f[3], r*f[4], r*f[5], r*f[6], r*f[7], r*f[8], r*f[9], r*f[10], r*f[11], r*f[12], r*f[13], r*f[14], r*f[15]);
        }
        void Transpose() {
            float f_tmp[16];
            memcpy_s(f_tmp, 16 * sizeof(float), f, 16 * sizeof(float));

            this->f[1] = f_tmp[4];
            this->f[2] = f_tmp[8];
            this->f[3] = f_tmp[12];
            this->f[4] = f_tmp[1];
            this->f[6] = f_tmp[9];
            this->f[7] = f_tmp[13];
            this->f[8] = f_tmp[2];
            this->f[9] = f_tmp[6];
            this->f[11] = f_tmp[14];
            this->f[12] = f_tmp[3];
            this->f[13] = f_tmp[7];
            this->f[14] = f_tmp[11];
        }
    };

    struct OVertex {
    private:
        OVector3 position;
        OVector2 texture;
        OVector3 normal;

    public:
        OVertex() {

        }
        OVertex(OVector3 position, OVector2 texture, OVector3 normal) {
            this->position = position;
            this->texture = texture;
            this->normal = normal;
        }

        void SetPosition(OVector3 position) { this->position = position; }
        void SetTexture(OVector2 texturePosition) { this->texture = texturePosition; }
        void SetNormal(OVector3 normal) { this->normal = normal; }

        OVector3 GetPosition() { return this->position; }
        OVector2 GetTexture() { return this->texture; }
        OVector3 GetNormal() { return this->normal; }
    };
    struct OFace {
    private:
        OVector3 data1; // f (1/2/3) 2/3/3 3/4/7
        OVector3 data2; // f 1/2/3 (2/3/3) 3/4/7
        OVector3 data3; // f 1/2/3 2/3/3 (3/4/7)

    public:
        OFace(OVector3 data1, OVector3 data2, OVector3 data3) {
            this->data1 = data1;
            this->data2 = data2;
            this->data3 = data3;
        }

        OFace() {
        }

        OFace(const OFace& rhs) {
            data1 = rhs.data1;
            data2 = rhs.data2;
            data3 = rhs.data3;
        }


        void SetData1(OVector3 data) { this->data1 = data; }
        void SetData2(OVector3 data) { this->data2 = data; }
        void SetData3(OVector3 data) { this->data3 = data; }

        OVector3 GetData1() { return this->data1; }
        OVector3 GetData2() { return this->data2; }
        OVector3 GetData3() { return this->data3; }
	};

	struct ViewBufferType	{
	public:
		OMatrix4x4 view;
	};

	struct MatrixBufferType	{
	public:
		OMatrix4x4 world;
		OMatrix4x4 view;
		OMatrix4x4 projection;
	};
    struct PixelBufferType {
        OVector4 pixelColor;
    };
    struct DiffuseLightBufferType {
        OVector4 diffuseColor;
        OVector3 lightDirection;
        float padding;   // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
    };
    struct AmbientLightBufferType {
        OVector4 ambientColor;
        OVector4 diffuseColor;
        OVector3 lightDirection;
        float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
    };

    struct ComponentMapping {
        float topLeft_U, topLeft_V, bottomRight_U, bottomRight_V;
    };

#pragma pack(push, 1)
    struct NetworkFrame {
        //1496 Byte Daten  ->  Entspricht nahezu dem Datengrößenformat eines Ethernet II Datenpaketes (1500 Byte)
        //1496 % 8 == 0    ->  Speicherschonend nach den Memory-Alignment Regeln

        long long checksum;
        long long authorisation;
        int id;
        int action;
        int senderID;
        int receiverID;
        char data[1464];

        bool operator==(const NetworkFrame *s2) {
            if (this->action == s2->action &&
                this->authorisation == s2->authorisation &&
                this->checksum == s2->checksum &&
                this->id == s2->id &&
                this->receiverID == s2->receiverID &&
                this->senderID == s2->senderID &&
                memcmp(&this->data, &s2->data, sizeof(char)* 1464) == 0)
                return true;

            else return false;
        }
    };
#pragma pack(pop)

}

#endif /* OSIRISIINTERNSTRUCTS_HPP_ */
