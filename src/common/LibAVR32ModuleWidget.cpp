#include "LibAVR32ModuleWidget.hpp"
#include "LibAVR32Module.hpp"
#include "VirtualGridModule.hpp"
#include "VirtualGridWidget.hpp"

using namespace rack;

struct ConnectGridItem : rack::ui::MenuItem
{
    LibAVR32Module* module;
    Grid* grid;

    void onAction(const rack::event::Action& e) override
    {
        if (module && module->gridConnection == grid)
        {
            GridConnectionManager::get()->disconnect(module, true);
        }
        else
        {
            GridConnectionManager::get()->connect(grid, module);
        }
    }
};

struct ReloadFirmwareItem : rack::ui::MenuItem
{
    LibAVR32Module* module;
    ReloadRequest requestType;

    void onAction(const rack::event::Action& e) override
    {
        if (module) {
            module->requestReloadFirmware(requestType);
        }
    }
};

struct ioRateItem : rack::ui::MenuItem
{
    int* target = nullptr;
    int defaultValue = 0;

    ui::Menu* createChildMenu() override
    {
        ui::Menu* menu = new ui::Menu;

        std::string names[] = { "1x", "/2", "/4", "/8", "/16" };
        std::string right[] = { "(audio rate)", "", "", "", "(lowest CPU)" };

        for (int i = 0; i < 5; i++)
        {
            int value = 1 << i;
            menu->addChild(createCheckMenuItem(
                names[i],
                (value == defaultValue) ? "(default)" : right[i],
                [=]()
                { return *target == value; },
                [=]()
                { *target = value; }));
        }

        return menu;
    }
};

struct FirmwareSubmenuItem : MenuItem
{
    LibAVR32Module* module;

    Menu* createChildMenu() override
    {
        LibAVR32Module* m = dynamic_cast<LibAVR32Module*>(module);
        assert(m);

        Menu* menu = new Menu;

        char versionbuf[512];
        module->firmware.getVersion(versionbuf);
        menu->addChild(construct<MenuLabel>(&MenuLabel::text, module->firmwareName));
        menu->addChild(construct<MenuLabel>(&MenuLabel::text, versionbuf));

        menu->addChild(construct<ioRateItem>(
            &MenuItem::text, "Input rate", &MenuItem::rightText, RIGHT_ARROW,
            &ioRateItem::defaultValue, 2, &ioRateItem::target, &m->inputRate));

        menu->addChild(construct<ioRateItem>(
            &MenuItem::text, "Output rate", &MenuItem::rightText, RIGHT_ARROW,
            &ioRateItem::defaultValue, 4, &ioRateItem::target, &m->outputRate));

        auto reloadItem = new ReloadFirmwareItem();
        reloadItem->text = "Reload & Restart";
        reloadItem->module = m;
        reloadItem->requestType = ReloadRequest::ReloadAndRestart;
        menu->addChild(reloadItem);

        auto hotReloadItem = new ReloadFirmwareItem();
        hotReloadItem->text = "Hot Reload";
        hotReloadItem->module = m;
        hotReloadItem->requestType = ReloadRequest::HotReload;
        menu->addChild(hotReloadItem);

        return menu;
    }
};

LibAVR32ModuleWidget::LibAVR32ModuleWidget()
{
}

void LibAVR32ModuleWidget::appendContextMenu(rack::Menu* menu)
{
    LibAVR32Module* m = dynamic_cast<LibAVR32Module*>(module);
    assert(m);

    menu->addChild(new MenuSeparator());

    auto firmwareMenu = new FirmwareSubmenuItem();
    firmwareMenu->text = "Firmware";
    firmwareMenu->module = m;
    firmwareMenu->rightText = "▸";
    menu->addChild(firmwareMenu);

    menu->addChild(new MenuSeparator());
    menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Device Connection"));

    // enumerate registered grid devices
    int deviceCount = 0;
    for (Grid* grid : GridConnectionManager::get()->getGrids())
    {
        auto connectItem = new ConnectGridItem();
        connectItem->text = grid->getDevice().type + " (" + grid->getDevice().id + ")";
        connectItem->rightText = (m->gridConnection && m->gridConnection->getDevice().id == grid->getDevice().id) ? "✔" : "";
        connectItem->module = m;
        connectItem->grid = grid;
        menu->addChild(connectItem);
        deviceCount++;
    }

    if (deviceCount == 0)
    {
        menu->addChild(construct<MenuLabel>(&MenuLabel::text, "(no physical or virtual devices found)"));
    }
}