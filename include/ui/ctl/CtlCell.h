/*
 * CtlCell.h
 *
 *  Created on: 17 июл. 2017 г.
 *      Author: sadko
 */

#ifndef UI_CTL_CTLCELL_H_
#define UI_CTL_CTLCELL_H_

namespace lsp
{
    namespace ctl
    {
        
        class CtlCell: public CtlWidget
        {
            public:
                explicit CtlCell(CtlRegistry *src, LSPCell *cell);
                virtual ~CtlCell();

            public:
                virtual void set(widget_attribute_t att, const char *value);

                virtual status_t add(LSPWidget *child);
        };
    
    } /* namespace ctl */
} /* namespace lsp */

#endif /* UI_CTL_CTLCELL_H_ */
