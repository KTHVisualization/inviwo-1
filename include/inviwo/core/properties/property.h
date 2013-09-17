#ifndef IVW_PROPERTY_H
#define IVW_PROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/properties/propertywidget.h>
#include <inviwo/core/properties/propertysemantics.h>
#include <inviwo/core/util/callback.h>
#include <inviwo/core/util/variant.h>
#include <inviwo/core/util/observer.h>

namespace inviwo {

class IVW_CORE_API Property : public IvwSerializable, public VoidObservable {

public:
    Property(std::string identifier, std::string displayName,
             PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
             PropertySemantics::Type semantics=PropertySemantics::Default);
    Property();
    
    virtual std::string getIdentifier() const;
    virtual void setIdentifier(const std::string& identifier);

    virtual std::string getClassName()  const { return "undefined"; }    

    virtual std::string getDisplayName() const;
    virtual void setDisplayName(const std::string& displayName);

    virtual PropertySemantics::Type getSemantics() const;
    virtual void setSemantics(const PropertySemantics::Type& semantics);

    PropertyOwner::InvalidationLevel getInvalidationLevel() const { return invalidationLevel_; }
    void setInvalidationLevel(PropertyOwner::InvalidationLevel invalidationLevel) {
        invalidationLevel_ = invalidationLevel;
    }

    PropertyOwner* getOwner();
    virtual void setOwner(PropertyOwner* owner);

    void registerPropertyWidget(PropertyWidget* propertyWidget);
    void updatePropertyWidgets();
    bool hasProcessorWidgets() { return (propertyWidgets_.size()!= 0); }

    void setGroupID(std::string groupID) { 
        groupID_ = groupID;
    }

    static void setGroupDisplayName(std::string groupID,std::string groupDisplayName);

    std::string getGroupID() { return groupID_; }
    std::string getGroupDisplayName();
    void hidePropertyGroup() { notifyObservers(); }
    void showPropertyGroup() { notifyObservers(); }

    virtual inline void propertyModified() { setPropertyModified(true); }
    virtual void setPropertyModified(bool modified) { propertyModified_ = modified; }
    virtual bool isPropertyModified() const { return propertyModified_; }

    void setVisible(bool visible);
    bool getVisible(){ return visible_; };

    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    bool operator== (Property* prop);

    template <typename T>
    void onChange(T* o, void (T::*m)()) {
        onChangeCallback_.addMemberFunction(o,m);
    }


protected:
    SingleCallBack onChangeCallback_;

private:
    std::string identifier_;
    std::string displayName_;
    PropertyOwner::InvalidationLevel invalidationLevel_;
    PropertySemantics::Type semantics_;

    bool propertyModified_;
    PropertyOwner* owner_;
    std::vector<PropertyWidget*> propertyWidgets_;

    bool visible_;
    std::string groupID_;
    std::string groupDisplayName_;

    static std::map<std::string,std::string> groupDisplayNames_;
};

} // namespace

#endif // IVW_PROPERTY_H