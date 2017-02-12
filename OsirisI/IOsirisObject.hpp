#ifndef IOSIRISOBJECT_HPP_
#define IOSIRISOBJECT_HPP_

namespace OsirisI {
	class IOsirisObject {
	private:
	protected:
		std::mutex lock;
		OsirisI::Utilities::HashedString name;

		public:
			IOsirisObject(OsirisI::Utilities::HashedString name) {
				this->name = name;
			}
			~IOsirisObject();

			virtual ReturnState Persist(OsirisI::Utilities::Persistor* persistor) {
				OS_CHECKSTATE(this->name.Persist(persistor));
				return OS_OK;
			}
			virtual ReturnState Unpersist(OsirisI::Utilities::Persistor* persistor) {
				OS_CHECKSTATE(this->name.Unpersist(persistor));
				return OS_OK;
			}

	        OsirisI::Utilities::HashedString GetName() {
		        return this->name;
			}

	        long GetNameHash() {
		        return this->name.GetHash();
			}

	        /*void SetName(OsirisI::Utilities::HashedString name) {
		        this->name = name;
			}*/
    };
}

#endif /* IOSIRISOBJECT_HPP_ */
