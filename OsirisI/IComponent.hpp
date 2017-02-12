#ifndef ICOMPONENT_HPP_
#define ICOMPONENT_HPP_

namespace OsirisI {
    namespace Graphics {
        namespace Components {
            /**
            * \brief The IComponent class is designed as a basic interface from which every gui element is derived.
            */
            class IComponent : public OsirisI::IOsirisObject {
                private:
                protected:
					IComponent(OsirisI::Utilities::HashedString name) : IOsirisObject(name) {}

                    OsirisI::OVector3 position, rotation;
                    OsirisI::OVector2 size;
                    OsirisI::Graphics::Renderer::RenderInfo* renderInfo;

                public:
                    OsirisI::OVector3 GetPosition() {
                        return this->position;
                    }
                    void SetPosition(OsirisI::OVector3 position) {
                        this->position = position;
                    }
                    OsirisI::OVector3 GetRotation() {
                        return this->rotation;
                    }
                    void SetRotation(OsirisI::OVector3 rotation) {
                        this->rotation = rotation;
                    }

                    
                    /**
                    * \brief This method is used to perform initialization operations which cannot be processed in constructor due to possible errors.
                    * \return Statuscode => 0 is OS_OK
                    */
					virtual ReturnState Initialize() = 0;
                    
                    /**
                    * \brief This method is used to update an object. It's called once per frame.
                    * \param delta Contains the time passed since the last update (in seconds)
                    * \return Statuscode => 0 is OS_OK
                    */
					virtual ReturnState Update(float delta) = 0;
                    
                    /**
                    * \brief This method is used to recover an objects internal state after something 
                    * \return Statuscode => 0 is OS_OK
                    */
					virtual ReturnState Recover() = 0;
                    
                    /**
                    * \brief This method is used to release all resources and memory allocated over the lifetime of the object
                    * \return Statuscode => 0 is OS_OK
                    */
					virtual ReturnState Release() = 0;
                    
                    /**
                    * \brief This method is used to get the RenderInfo provided by every renderable object.
                    * \return RenderInfo => nullptr is error or empty
                    */
					virtual OsirisI::Graphics::Renderer::RenderInfo* GetRenderInfo() = 0;
            };
        }
    }
}

#endif /* ICOMPONENT_HPP_ */
