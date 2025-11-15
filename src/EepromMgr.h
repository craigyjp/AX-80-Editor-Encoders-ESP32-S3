#include <EEPROM.h>

#define EEPROM_MIDI_CH 0
#define EEPROM_ENCODER_DIR 1
#define EEPROM_LAST_PATCH 2
#define EEPROM_MIDI_OUT_CH 3
#define EEPROM_LOAD_FACTORY 4
#define EEPROM_UPDATE_PARAMS 5
#define EEPROM_SAVE_CURRENT 6
#define EEPROM_SAVE_ALL 7
#define EEPROM_ROM_TYPE 8
#define EEPROM_LOAD_RAM 9
#define EEPROM_BANK_SELECT 10
#define EEPROM_ENCODER_ACCELERATE 11
#define EEPROM_AFTERTOUCH 12
#define EEPROM_ENCODER_STEP_MODE 13

int getMIDIChannel() {
  byte midiChannel = EEPROM.read(EEPROM_MIDI_CH);
  if (midiChannel < 0 || midiChannel > 16) midiChannel = MIDI_CHANNEL_OMNI;//If EEPROM has no MIDI channel stored
  return midiChannel;
}

void storeMidiChannel(byte channel)
{
  EEPROM.write(EEPROM_MIDI_CH, channel);
  EEPROM.commit();
}

boolean getEncoderDir() {
  byte ed = EEPROM.read(EEPROM_ENCODER_DIR); 
  if (ed < 0 || ed > 1)return true; //If EEPROM has no encoder direction stored
  return ed == 1 ? true : false;
}

void storeEncoderDir(byte encoderDir)
{
  EEPROM.write(EEPROM_ENCODER_DIR, encoderDir);
  EEPROM.commit();
}

boolean getEncoderAccelerate() {
  byte ea = EEPROM.read(EEPROM_ENCODER_ACCELERATE);
  if (ea > 1) return false;
  return ea == 1;
}

void storeEncoderAccelerate(byte encoderAccelerate)
{
  EEPROM.write(EEPROM_ENCODER_ACCELERATE, encoderAccelerate);
  EEPROM.commit();
}

boolean getUpdateParams() {
  byte params = EEPROM.read(EEPROM_UPDATE_PARAMS); 
  if (params < 0 || params > 1)return true; //If EEPROM has no encoder direction stored
  return params == 1 ? true : false;
}

void storeUpdateParams(byte updateParameters)
{
  EEPROM.write(EEPROM_UPDATE_PARAMS, updateParameters);
  EEPROM.commit();
}

int getLastPatch() {
  int lastPatchNumber = EEPROM.read(EEPROM_LAST_PATCH);
  if (lastPatchNumber < 1 || lastPatchNumber > 999) lastPatchNumber = 1;
  return lastPatchNumber;
}

void storeLastPatch(int lastPatchNumber)
{
  EEPROM.write(EEPROM_LAST_PATCH, lastPatchNumber);
  EEPROM.commit();
}

int getMIDIOutCh() {
  byte mc = EEPROM.read(EEPROM_MIDI_OUT_CH);
  if (mc < 0 || midiOutCh > 16) mc = 0;//If EEPROM has no MIDI channel stored
  return mc;
}

void storeMidiOutCh(byte midiOutCh){
  EEPROM.write(EEPROM_MIDI_OUT_CH, midiOutCh);
  EEPROM.commit();
}

int getSetBank() {
  byte sb = EEPROM.read(EEPROM_BANK_SELECT);
  if (sb < 0 || sb > 4) sb = 0;//If EEPROM has no MIDI channel stored
  return sb;
}

void storeSetBank(byte sb){
  EEPROM.write(EEPROM_BANK_SELECT, sb);
  EEPROM.commit();
}

boolean getLoadFactory() {
  byte lf = EEPROM.read(EEPROM_LOAD_FACTORY); 
  if (lf < 0 || lf > 1)return true;
  return lf ? true : false;
}

void storeLoadFactory(byte lfupdate)
{
  EEPROM.write(EEPROM_LOAD_FACTORY, lfupdate);
  EEPROM.commit();
}

boolean getROMType() {
  byte rt = EEPROM.read(EEPROM_ROM_TYPE); 
  if (rt < 0 || rt > 1)return true;
  return rt ? true : false;
}

void storeROMType(byte rtupdate)
{
  EEPROM.write(EEPROM_ROM_TYPE, rtupdate);
  EEPROM.commit();
}

boolean getLoadRAM() {
  byte lr = EEPROM.read(EEPROM_LOAD_RAM);
  if (lr > 1) return false;
  return lr == 1;
}

void storeLoadRAM(byte lrupdate)
{
  EEPROM.write(EEPROM_LOAD_RAM, lrupdate);
  EEPROM.commit();
}

boolean getSaveCurrent() {
  byte sc = EEPROM.read(EEPROM_SAVE_CURRENT); 
  if (sc < 0 || sc > 1)return true;
  return sc ? true : false;
}

void storeSaveCurrent(byte scupdate)
{
  EEPROM.write(EEPROM_SAVE_CURRENT, scupdate);
  EEPROM.commit();
}

boolean getAfterTouch() {
  byte at = EEPROM.read(EEPROM_AFTERTOUCH); 
  if (at < 0 || at > 1)return false;
  return at ? true : false;
}

void storeAfterTouch(byte atupdate)
{
  EEPROM.write(EEPROM_AFTERTOUCH, atupdate);
  EEPROM.commit();
}

boolean getSaveAll() {
  byte sa = EEPROM.read(EEPROM_SAVE_ALL);
  if (sa > 1) return false;
  return sa == 1;
}

void storeSaveAll(byte saupdate)
{
  EEPROM.write(EEPROM_SAVE_ALL, saupdate);
  EEPROM.commit();
}

boolean getEncoderStepMode() {
  byte sm = EEPROM.read(EEPROM_ENCODER_STEP_MODE);
  if (sm > 1) return false; // default = full-step
  return sm == 1;
}

void storeEncoderStepMode(byte mode) {
  EEPROM.write(EEPROM_ENCODER_STEP_MODE, mode);
  EEPROM.commit();
}
