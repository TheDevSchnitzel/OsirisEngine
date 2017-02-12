#include "stdafx.h"

namespace OsirisI {
	namespace Utilities {
		HashedString::HashedString() {
			str = "";
			this->hash = 0;
		}

		HashedString::HashedString(std::string str) : str(str) {
            if(str == "") {
                this->hash = 0; 
                return;
            }

			unsigned int s1 = 1;
			unsigned int s2 = 0;

			for (size_t n = 0; n < str.length(); n++) {
				s1 = (s1 + str[n]) % 65521;
				s2 = (s2 + s1) % 65521;
			}     
			this->hash = s2 ^ s1;
		}
		HashedString::~HashedString() {
		}

        long HashedString::GetByteSize() {
            return this->str.length() + sizeof(long);
        }

		std::string HashedString::GetString() const {
			return this->str;
		}

		long HashedString::GetHash() const {
			return this->hash;
		}        
        
        bool operator!=(const HashedString& lhs, const HashedString& rhs) {
            return lhs.GetHash() == rhs.GetHash();
        }

        bool operator==(const HashedString& lhs, const HashedString& rhs) {
            return lhs.GetHash() == rhs.GetHash();
        }

        bool operator<=(const HashedString& lhs, const HashedString& rhs) {
            return lhs.GetHash() <= rhs.GetHash();
        }

        bool operator>=(const HashedString& lhs, const HashedString& rhs) {
            return lhs.GetHash() >= rhs.GetHash();
        }

        bool operator<(const HashedString& lhs, const HashedString& rhs) {
            return lhs.GetHash() < rhs.GetHash();
        }

        bool operator>(const HashedString& lhs, const HashedString& rhs) {
            return lhs.GetHash() > rhs.GetHash();
        }

        std::ostream& operator<<(std::ostream& lhs, const HashedString& rhs) {
            return lhs << "{" << rhs.GetHash() << ": " << rhs.GetString() << "}";
        }
	}
}