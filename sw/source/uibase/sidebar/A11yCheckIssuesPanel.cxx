/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; fill-column: 100 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sal/config.h>

#include <AccessibilityCheck.hxx>
#include <cmdid.h>
#include <doc.hxx>
#include <docsh.hxx>
#include <ndtxt.hxx>
#include <wrtsh.hxx>

#include <sfx2/AccessibilityIssue.hxx>
#include <vcl/svapp.hxx>

#include "A11yCheckIssuesPanel.hxx"

#include <com/sun/star/lang/IllegalArgumentException.hpp>

namespace sw::sidebar
{
AccessibilityCheckEntry::AccessibilityCheckEntry(
    weld::Container* pParent, std::shared_ptr<sfx::AccessibilityIssue> const& rAccessibilityIssue)
    : m_xBuilder(Application::CreateBuilder(pParent, "svx/ui/accessibilitycheckentry.ui"))
    , m_xContainer(m_xBuilder->weld_container("accessibilityCheckEntryBox"))
    , m_xLabel(m_xBuilder->weld_label("accessibilityCheckEntryLabel"))
    , m_xGotoButton(m_xBuilder->weld_button("accessibilityCheckEntryGotoButton"))
    , m_pAccessibilityIssue(rAccessibilityIssue)
{
    m_xLabel->set_label(m_pAccessibilityIssue->m_aIssueText);
    // lock in the height as including the button so all rows are the same height
    m_xContainer->set_size_request(-1, m_xContainer->get_preferred_size().Height());
    m_xGotoButton->set_visible(m_pAccessibilityIssue->canGotoIssue());
    m_xGotoButton->connect_clicked(LINK(this, AccessibilityCheckEntry, GotoButtonClicked));
}

IMPL_LINK_NOARG(AccessibilityCheckEntry, GotoButtonClicked, weld::Button&, void)
{
    m_pAccessibilityIssue->gotoIssue();
}

std::unique_ptr<PanelLayout> A11yCheckIssuesPanel::Create(weld::Widget* pParent,
                                                          SfxBindings* pBindings)
{
    if (pParent == nullptr)
        throw ::com::sun::star::lang::IllegalArgumentException(
            "no parent window given to A11yCheckIssuesPanel::Create", nullptr, 0);
    return std::make_unique<A11yCheckIssuesPanel>(pParent, pBindings);
}

A11yCheckIssuesPanel::A11yCheckIssuesPanel(weld::Widget* pParent, SfxBindings* pBindings)
    : PanelLayout(pParent, "A11yCheckIssuesPanel", "modules/swriter/ui/a11ycheckissuespanel.ui")
    , m_xAccessibilityCheckBox(m_xBuilder->weld_box("accessibilityCheckBox"))
    , m_xScrolledWindow(m_xBuilder->weld_scrolled_window("scrolledwindow"))
    , mpDoc(nullptr)
    , maA11yCheckController(FN_STAT_ACCESSIBILITY_CHECK, *pBindings, *this)
    , mnIssueCount(0)
{
    SwDocShell* pDocSh = dynamic_cast<SwDocShell*>(SfxObjectShell::Current());
    if (!pDocSh)
        return;

    mpDoc = pDocSh->GetDoc();

    populateIssues();
}

A11yCheckIssuesPanel::~A11yCheckIssuesPanel() { m_xAccessibilityCheckBox.reset(); }

void A11yCheckIssuesPanel::populateIssues()
{
    if (!mpDoc)
        return;
    sw::AccessibilityCheck aCheck(mpDoc);
    aCheck.check();
    m_aIssueCollection = aCheck.getIssueCollection();

    // Remove old issue widgets
    for (auto const& xEntry : m_aAccessibilityCheckEntries)
        m_xAccessibilityCheckBox->move(xEntry->get_widget(), nullptr);

    sal_Int32 i = 0;
    for (std::shared_ptr<sfx::AccessibilityIssue> const& pIssue : m_aIssueCollection.getIssues())
    {
        auto xEntry
            = std::make_unique<AccessibilityCheckEntry>(m_xAccessibilityCheckBox.get(), pIssue);
        m_xAccessibilityCheckBox->reorder_child(xEntry->get_widget(), i++);
        m_aAccessibilityCheckEntries.push_back(std::move(xEntry));
    }

    if (!m_aAccessibilityCheckEntries.empty())
    {
        auto nRowHeight
            = m_aAccessibilityCheckEntries.back()->get_widget()->get_preferred_size().Height();
        // 6 is the spacing set in the .ui
        m_xScrolledWindow->vadjustment_set_step_increment(nRowHeight + 6);
    }
}

void A11yCheckIssuesPanel::NotifyItemUpdate(const sal_uInt16 nSid, const SfxItemState /* eState */,
                                            const SfxPoolItem* pState)
{
    if (!m_xAccessibilityCheckBox) //disposed
        return;

    switch (nSid)
    {
        case FN_STAT_ACCESSIBILITY_CHECK:
        {
            sal_Int32 nIssueCount = static_cast<const SfxInt32Item*>(pState)->GetValue();
            if (nIssueCount != mnIssueCount)
            {
                mnIssueCount = nIssueCount;
                populateIssues();
            }
        }
        break;
        default:
            break;
    }
}
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab cinoptions=b1,g0,N-s cinkeys+=0=break: */
