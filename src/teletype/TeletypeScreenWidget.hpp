#include "rack.hpp"
#include "LibAVR32Module.hpp"

using namespace rack;

struct TeletypeScreenWidget : OpaqueWidget
{
    TeletypeScreenWidget(LibAVR32Module* module);

    virtual void onSelect(const event::Select& e) override;
    virtual void onDeselect(const event::Deselect& e) override;
    virtual void onSelectKey(const event::SelectKey& e) override;
    virtual void draw(const DrawArgs& args) override;
    virtual void drawLayer(const DrawArgs& args, int layer) override;

protected:
    void drawFrame(NVGcontext* vg);
    void drawPixels(NVGcontext* vg);
    void drawPixel(NVGcontext* vg, float x, float y, float width, float height, int data);

    LibAVR32Module* module;
};