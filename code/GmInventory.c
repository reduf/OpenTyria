#pragma once

void GmBag_InitBackpack(GmBag *bag, uint16_t bag_id)
{
    bag->bag_id = bag_id;
    bag->bag_model_id = BagModelId_Backpack;
    bag->bag_type = BagType_Bag;
    bag->slot_count = 20;
}

void GmBag_InitUnclaimedItems(GmBag *bag, uint16_t bag_id)
{
    bag->bag_id = bag_id;
    bag->bag_model_id = BagModelId_UnclaimedItems;
    bag->bag_type = BagType_NotCollected;
    bag->slot_count = 12;
}

void GmBag_InitEquippedItems(GmBag *bag, uint16_t bag_id)
{
    bag->bag_id = bag_id;
    bag->bag_model_id = BagModelId_EquippedItems;
    bag->bag_type = BagType_Equiped;
    bag->slot_count = 9;
}
