/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2018 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *********************************************************************************/

#ifndef IVW_SIMPLELIGHTINGPROPERTY_H
#define IVW_SIMPLELIGHTINGPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/templateproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/optionproperty.h>
#include <inviwo/core/properties/lightproperty.h>

namespace inviwo {

namespace ShadingMode {
    enum Modes {
        None,
        Ambient,
        Diffuse,
        Specular,
        BlinnPhong,
        Phong,
		OrenNayar,
		OrenNayarDiffuse
    };
}

class CameraProperty;
/**
 * \ingroup properties
 * A CompositeProperty representing a light with position, ambient, diffuse, specular color. Used
 * for phong shading.
 */
class IVW_CORE_API SimpleLightingProperty : public CompositeProperty {
public:
    InviwoPropertyInfo();
	static const size_t MAX_NUMBER_OF_LIGHTS = 8;

    SimpleLightingProperty(std::string identifier, std::string displayName,
                           CameraProperty* camera = nullptr,
                           InvalidationLevel = InvalidationLevel::InvalidResources,
                           PropertySemantics semantics = PropertySemantics::Default);
    
    SimpleLightingProperty(const SimpleLightingProperty& rhs);
    SimpleLightingProperty& operator=(const SimpleLightingProperty& that);
    virtual SimpleLightingProperty* clone() const override;
    virtual ~SimpleLightingProperty();
    
    // Light properties
    OptionPropertyInt shadingMode_;
    OptionPropertyInt referenceFrame_;

    FloatProperty specularExponent_;
	FloatProperty roughness_;

	void addLight();
	void deleteLight();
	size_t numLights_;
	OptionPropertyString lightSelection_;
	ButtonProperty addLight_;
	ButtonProperty deleteLight_;
    
	const CameraProperty* getCameraProperty() const {
		return camera_;
	}
private:
    CameraProperty* camera_; //< Non-owning reference.
};

} // namespace

#endif // IVW_SIMPLELIGHTINGPROPERTY_H

