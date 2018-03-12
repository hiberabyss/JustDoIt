An example of IPC via shared memory. Build via `make`.

The following two processes will save their pid into the shared memory first,
then each will get another's pid via the shared memory:

* writer: write data into shared memory buffer; when get "quit", will exit;
* reader: read data from shared memory; when get "quit", will exit;
