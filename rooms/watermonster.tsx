<?xml version="1.0" encoding="UTF-8"?>
<tileset name="watermonster" tilewidth="20" tileheight="32" tilecount="16" columns="8">
 <image source="watermonster_20x32.png" trans="ff00da" width="160" height="64"/>
 <tile id="0">
  <properties>
   <property name="state" value="out"/>
  </properties>
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="3" duration="250"/>
   <frame tileid="4" duration="333"/>
   <frame tileid="5" duration="333"/>
   <frame tileid="6" duration="333"/>
   <frame tileid="7" duration="300"/>
  </animation>
 </tile>
 <tile id="7">
  <properties>
   <property name="state" value="in"/>
  </properties>
  <animation>
   <frame tileid="7" duration="100"/>
   <frame tileid="6" duration="100"/>
   <frame tileid="5" duration="100"/>
   <frame tileid="4" duration="100"/>
   <frame tileid="3" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="0" duration="100"/>
   <frame tileid="8" duration="100"/>
  </animation>
 </tile>
 <tile id="10">
  <properties>
   <property name="state" value="idle"/>
  </properties>
  <animation>
   <frame tileid="10" duration="100"/>
   <frame tileid="11" duration="100"/>
   <frame tileid="12" duration="100"/>
   <frame tileid="11" duration="100"/>
  </animation>
 </tile>
</tileset>
