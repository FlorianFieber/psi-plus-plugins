/*
 * psi-otr.h - off-the-record messaging plugin for psi
 *
 * Copyright (C) Timo Engel (timo-e@freenet.de), Berlin 2007.
 * This program was written as part of a diplom thesis advised by 
 * Prof. Dr. Ruediger Weis (PST Labor)
 * at the Technical University of Applied Sciences Berlin.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef PSIOTRPLUGIN_HPP_
#define PSIOTRPLUGIN_HPP_

#include <QObject>
#include <QtGui>
#include <QDomElement>

#include "OtrMessaging.hpp"
#include "psiplugin.h"
#include "eventfilter.h"
#include "optionaccessinghost.h"
#include "optionaccessor.h"
#include "stanzasender.h"
#include "stanzasendinghost.h"
#include "applicationinfoaccessor.h"
#include "stanzafilter.h"
#include "toolbariconaccessor.h"
#include "contactinfoaccessinghost.h"
#include "contactinfoaccessor.h"

class ApplicationInfoAccessingHost;

namespace psiotr
{

class ConfigDlg;
class PsiOtrClosure;
    
//-----------------------------------------------------------------------------

class PsiOtrPlugin : public QObject,
                     public PsiPlugin,
                     public EventFilter,
                     public OptionAccessor,
                     public StanzaSender,
                     public ApplicationInfoAccessor,
                     public StanzaFilter,
                     public ToolbarIconAccessor,
                     public ContactInfoAccessor,
                     public OtrCallback
{
Q_OBJECT
Q_INTERFACES(PsiPlugin
             EventFilter
             OptionAccessor
             StanzaSender
             ApplicationInfoAccessor
             StanzaFilter
             ToolbarIconAccessor
             ContactInfoAccessor)

public:
    PsiOtrPlugin();
    ~PsiOtrPlugin();

    // PsiPlugin
    virtual QString name() const;
    virtual QString shortName() const;
    virtual QString version() const;
    virtual QWidget* options(); 
    virtual bool enable();
    virtual bool disable();
    virtual void applyOptions();
    virtual void restoreOptions();

    // EventFilter
    virtual bool processEvent(int account, QDomElement& e);
    virtual bool processMessage(int account, const QString& fromJid,
                                const QString& body, const QString& subject);
    virtual bool processOutgoingMessage(int account, const QString& toJid,
                                        QString& body, const QString& type,
                                        QString& subject);
    virtual void logout(int account);

    // OptionAccessor
    virtual void setOptionAccessingHost(OptionAccessingHost* host);
    virtual void optionChanged(const QString& option);

    // StanzaSender
    virtual void setStanzaSendingHost(StanzaSendingHost *host);

    // ApplicationInfoAccessor
    virtual void setApplicationInfoAccessingHost(ApplicationInfoAccessingHost* host);

    // StanzaFilter
    virtual bool incomingStanza(int account, const QDomElement& xml);
    virtual bool outgoingStanza(int account, QDomElement &xml);

    // ToolbarIconAccessor
    virtual QList<QVariantHash> getButtonParam();
    virtual QAction* getAction(QObject* parent, int account,
                               const QString& contact);

    // ContactInfoAccessor
    virtual void setContactInfoAccessingHost(ContactInfoAccessingHost* host);

    // OtrCallback
    virtual QString dataDir();
    virtual void sendMessage(const QString& account, const QString& toJid,
                             const QString& message);
    virtual bool isLoggedIn(const QString& account, const QString& jid);
    virtual void notifyUser(const OtrNotifyType& type, const QString& message);
    virtual void stopMessages();
    virtual void startMessages();

private:
    /**
     * Returns full Jid for private contacts,
     * bare Jid for non-private contacts.
     */
    QString getCorrectJid(int account, const QString& fullJid);

    bool                                            m_enabled;
    OtrMessaging*                                   m_otrConnection;
    QHash<QString, QHash<QString, PsiOtrClosure*> > m_onlineUsers;
    QString                                         m_psiDataDir;
    OptionAccessingHost*                            m_optionHost;
    StanzaSendingHost*                              m_senderHost;
    ApplicationInfoAccessingHost*                   m_applicationInfo;
    ContactInfoAccessingHost*                       m_contactInfo;
};

//-----------------------------------------------------------------------------

} // namespace psiotr

#endif