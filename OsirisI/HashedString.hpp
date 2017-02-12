#ifndef HASHEDSTRING_HPP_
#define HASHEDSTRING_HPP_

namespace OsirisI {
	namespace Utilities {
		class HashedString {
			protected:
				std::string str;
				long hash;

			public:
				HashedString();
				HashedString(std::string str);
				~HashedString();
			
                long GetByteSize();

				std::string GetString() const;
				long GetHash() const;

				virtual ReturnState Persist(OsirisI::Utilities::Persistor* persistor) {
					OS_CHECKSTATE(persistor->Persist(this->hash, sizeof(long)));
					OS_CHECKSTATE(persistor->Persist(this->str.length(), sizeof(std::string::size_type)));
					OS_CHECKSTATE(persistor->Persist(this->str, this->str.length()));
					return 0x0;
				}
				virtual ReturnState Unpersist(OsirisI::Utilities::Persistor* persistor) {
					OS_CHECKSTATE(persistor->Persist(hash, sizeof(long)));
					std::string::size_type nameSize = 0l;
					OS_CHECKSTATE(persistor->Persist(nameSize, sizeof(std::string::size_type)));
					OS_CHECKSTATE(persistor->Persist(str, nameSize));
					return 0x0;
				}
        };
        
        //Operatoren auf diese Weise zu ueberladen verhindert C2678/C2662/LNK4006
        bool operator!=(const HashedString& lhs, const HashedString& rhs);
        bool operator==(const HashedString& lhs, const HashedString& rhs);
        bool operator<=(const HashedString& lhs, const HashedString& rhs);
        bool operator>=(const HashedString& lhs, const HashedString& rhs);
        bool operator<(const HashedString& lhs, const HashedString& rhs);
        bool operator>(const HashedString& lhs, const HashedString& rhs);
        std::ostream& operator<<(std::ostream& lhs, const HashedString& rhs);
    }
}

#endif /* HASHEDSTRING_HPP_ */
