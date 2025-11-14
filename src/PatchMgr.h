#ifndef PATCHMGR_H
#define PATCHMGR_H

#include <LittleFS.h>
#include <CircularBuffer.hpp>

// Your existing defines and constants...
#define TOTALCHARS 63

const char CHARACTERS[TOTALCHARS] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};



// Function declarations
size_t readField(File *file, char *str, size_t size, const char *delim);
void recallPatchData(File patchFile, String data[]);
void sortPatches();
void loadPatches();
void savePatch(const char *patchNo, String patchData);
void savePatch(const char *patchNo, String patchData[]);
void deletePatch(const char *patchNo);
void renumberPatchesOnSD();
void setPatchesOrdering(int no);
void resetPatchesOrdering();

#endif

size_t readField(File *file, char *str, size_t size, const char *delim)
{
  size_t n = 0;
  int ch;
  
  while ((n + 1) < size && (ch = file->read()) != -1)
  {
    // Delete CR.
    if (ch == '\r')
    {
      continue;
    }
    str[n++] = (char)ch;
    if (strchr(delim, (char)ch))
    {
      break;
    }
  }
  str[n] = '\0';
  return n;
}

void recallPatchData(File patchFile, String data[])
{
  //Read patch data from file and set current patch parameters
  size_t n;     // Length of returned field with delimiter.
  char str[20]; // Must hold longest field with delimiter and zero byte.
  int i = 0;
  while (patchFile.available() && i < NO_OF_PARAMS)
  {
    n = readField(&patchFile, str, sizeof(str), ",\n");
    // done if Error or at EOF.
    if (n == 0)
      break;
    // Print the type of delimiter.
    if (str[n - 1] == ',' || str[n - 1] == '\n')
    {
      // Remove the delimiter.
      str[n - 1] = 0;
    }
    else
    {
      // At eof, too long, or read error.  Too long is error.
      Serial.print(patchFile.available() ? F("error: ") : F("eof:   "));
    }
    // Print the field.
    //    Serial.print(i);
    //    Serial.print(" - ");
    //    Serial.println(str);
    data[i++] = String(str);
  }
}

int compare(const void *a, const void *b) {
  return ((PatchNoAndName*)a)->patchNo - ((PatchNoAndName*)b)->patchNo;
}

void sortPatches()
{
  int arraySize = patches.size();
  //Sort patches buffer to be consecutive ascending patchNo order
  struct PatchNoAndName arrayToSort[arraySize];

  for (int i = 0; i < arraySize; ++i)
  {
    arrayToSort[i] = patches[i];
  }
  qsort(arrayToSort, arraySize, sizeof(PatchNoAndName), compare);
  patches.clear();

  for (int i = 0; i < arraySize; ++i)
  {
    patches.push(arrayToSort[i]);
  }
}

void loadPatches()
{
  Serial.println("=== loadPatches() ===");
  
  // Create patches directory if it doesn't exist
  if (!LittleFS.exists("/patches")) {
    Serial.println("Creating /patches directory");
    LittleFS.mkdir("/patches");
  }
  
  File root = LittleFS.open("/patches");
  if (!root) {
    Serial.println("ERROR: Failed to open /patches directory");
    return;
  }
  
  patches.clear();
  
  File patchFile = root.openNextFile();
  while (patchFile)
  {
    if (!patchFile.isDirectory())
    {
      String data[NO_OF_PARAMS]; //Array of data read in
      recallPatchData(patchFile, data);
      
      // Extract patch number from filename (remove ".txt" extension)
      String filename = patchFile.name();
      //filename.replace(".txt", "");
      filename.replace("/patches/", "");
      int patchNumber = filename.toInt();
      
      patches.push(PatchNoAndName{patchNumber, data[0]});
      Serial.println(String(patchFile.name()) + ":" + data[0]);
    }
    patchFile.close();
    patchFile = root.openNextFile();
  }
  root.close();
  sortPatches();
  Serial.print("Loaded ");
  Serial.print(patches.size());
  Serial.println(" patches");
}

void savePatch(const char *patchNo, String patchData)
{
  Serial.println("=== savePatch(String) called ===");
  Serial.print("Patch number: '");
  Serial.print(patchNo);
  Serial.println("'");
  
  // Create a proper filename with path and extension
  String filename = "/patches/";
  filename += patchNo;
  //filename += ".txt";
  
  Serial.print("Full filename: '");
  Serial.print(filename);
  Serial.println("'");
  
  Serial.print("Data length: ");
  Serial.println(patchData.length());
  
  // Check if LittleFS is mounted
  if (!LittleFS.begin()) {
    Serial.println("ERROR: LittleFS not mounted!");
    return;
  }
  
  // Create patches directory if it doesn't exist
  if (!LittleFS.exists("/patches")) {
    Serial.println("Creating /patches directory");
    LittleFS.mkdir("/patches");
  }
  
  // Check if file exists
  if (LittleFS.exists(filename))
  {
    Serial.println("Deleting existing patch file");
    if (LittleFS.remove(filename)) {
      Serial.println("Successfully deleted existing file");
    } else {
      Serial.println("ERROR: Failed to delete existing file");
    }
  }
  
  // Try to open the file for writing
  Serial.print("Opening file '");
  Serial.print(filename);
  Serial.println("' for writing...");
  
  File patchFile = LittleFS.open(filename, "w");
  if (!patchFile) {
    Serial.println("ERROR: Failed to open file for writing");
    
    // Additional debugging - try to list root directory
    Serial.println("Listing root directory:");
    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while(file){
      Serial.print("Found: ");
      Serial.println(file.name());
      file.close();
      file = root.openNextFile();
    }
    root.close();
    return;
  }
  
  Serial.println("File opened successfully");
  
  // Try to write data
  size_t bytesWritten = patchFile.println(patchData);
  if (bytesWritten > 0) {
    Serial.print("Successfully wrote ");
    Serial.print(bytesWritten);
    Serial.println(" bytes");
  } else {
    Serial.println("ERROR: Failed to write data to file");
  }
  
  patchFile.close();
  Serial.println("File closed");
  
  // Verify the file was created
  if (LittleFS.exists(filename)) {
    Serial.println("SUCCESS: Patch file verified on filesystem");
  } else {
    Serial.println("ERROR: Patch file not found after writing!");
  }
  
  Serial.println("=== End savePatch(String) ===");
}

void savePatch(const char *patchNo, String patchData[])
{
  Serial.println("=== savePatch(String[]) called ===");
  Serial.print("Patch number: '");
  Serial.print(patchNo);
  Serial.println("'");
  
  String dataString = patchData[0];
  for (int i = 1; i < NO_OF_PARAMS; i++)
  {
    dataString = dataString + "," + patchData[i];
  }
  
  Serial.print("Combined data length: ");
  Serial.println(dataString.length());
  Serial.print("Combined data: ");
  Serial.println(dataString);
  
  // Call the other savePatch function
  savePatch(patchNo, dataString);
  
  Serial.println("=== End savePatch(String[]) ===");
}

void deletePatch(const char *patchNo)
{
  String filename = "/patches/";
  filename += patchNo;
  //filename += ".txt";
  
  if (LittleFS.exists(filename)) {
    LittleFS.remove(filename);
    Serial.println("Deleted: " + filename);
  } else {
    Serial.println("File not found for deletion: " + filename);
  }
}

void renumberPatchesOnSD() {
  for (int i = 0; i < patches.size(); i++)
  {
    String data[NO_OF_PARAMS]; //Array of data read in
    File file = LittleFS.open(String(patches[i].patchNo).c_str(), "r");  // Add "r" mode
    if (file) {
      recallPatchData(file, data);
      file.close();
      savePatch(String(i + 1).c_str(), data);
    }
  }
  deletePatch(String(patches.size() + 1).c_str()); //Delete final patch which is duplicate of penultimate patch
}

void setPatchesOrdering(int no) {
  if (patches.size() < 2)return;
  while (patches.first().patchNo != no) {
    patches.push(patches.shift());
  }
}

void resetPatchesOrdering() {
  setPatchesOrdering(1);
}
