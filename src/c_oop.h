#ifndef C_OOP_DEBUG__H
#define C_OOP_DEBUG__H

typedef struct {
  int (*m_derived)(void *_param);
  void *m_private;
} Base;

#endif