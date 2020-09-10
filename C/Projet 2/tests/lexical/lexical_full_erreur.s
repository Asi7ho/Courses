#TEST_RETURN_CODE=FAIL
.set noreorder

.text
  AD $2, $4, $3               #instruction invalide
.data
  ,byte 3                     #, invalide
  .byth 3                     #directive invalide
  ::etiq                      #erreur ecriture
